/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadingScene.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 21:26:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 14:07:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOADINGSCENE_HPP
# define LOADINGSCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"
# include "Image.hpp"
# include "Text.hpp"
# include "LimitedText.hpp"
# include "BackgroundImage.hpp"

# include "ShaderManager.hpp"
# include "TextureManager.hpp"
# include "Game.hpp"

class LoadingScene : public Scene
{
	public:
		LoadingScene(Game *game, Scene *scene) : Scene(game)
		{
			_id = "LoadingScene";
			_loading = scene;
		}
		~LoadingScene() {};

		void onEnter()
		{
			TextureManager &textures = _game->getTextures();
			ShaderManager &shaders = _game->getShaders();

			_panel.add("background", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5));
			_panel.add("loading_text", new Text("Loading", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 0), glm::vec2(0.5)));

			_loadingDone = false;
			_thread = std::thread([this]()
				{
					_loading->onEnter();

					_loadingDone = true;
				});
		}

		void processInput(float ) {}
		void update(float )
		{
			if (_loadingDone)
			{
				_thread.join();
				requestScene(_loading);
				return ;
			}

			Text	*loadingScreenText = static_cast<Text*>(_panel.get("loading_text"));

			static double last = 0;
			if (glfwGetTime() - last > 0.3)
			{
				std::string str = loadingScreenText->getText();
				str += '.';
				if (str == "Loading....")
					str = "Loading";
				loadingScreenText->setText(str);

				last = glfwGetTime();
			}
		}
		void render()
		{
			Window	&window = _game->getWindow();

			glDisable(GL_DEPTH_TEST);

			_panel.draw(window.getSize());
		}

		void onExit() {}

	private:
		Panel				_panel;

		std::atomic<bool>	_loadingDone;
		std::thread			_thread;

		Scene				*_loading;
};

#endif
