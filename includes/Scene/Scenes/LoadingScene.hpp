/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadingScene.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 21:26:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 21:54:08 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOADINGSCENE_HPP
# define LOADINGSCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"
# include "Image.hpp"
# include "Text.hpp"

class LoadingScene : public Scene
{
	public:
		LoadingScene(Game *game, Scene *scene) : Scene(game), _loadingDone(false)
		{
			_loading = scene;
		}
		~LoadingScene() {};

		void onEnter()
		{
			TextureManager &textures = _game->getTextures();

			_loadingScreen = new Image(textures.get("ft_minecraft"), glm::vec2(0, -32), glm::vec2(0.5, 0.5), glm::vec2(0.25));
			_loadingScreenText = new Text("Loading", textures.get("ascii"), glm::vec2(0, 0), glm::vec2(0.5));

			_thread = std::thread([this]()
				{
					_loading->onEnter();
					_loadingDone = true;
				});
			_thread.detach();
		}

		void processInput(float ) {}
		void update(float )
		{
			if (_loadingDone)
			{
				requestScene(_loading);
			}

			Text	*loadingScreenText = static_cast<Text*>(_loadingScreenText);

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
			ShaderManager &shaders = _game->getShaders();
			Window	&window = _game->getWindow();

			glDisable(GL_DEPTH_TEST);

			shaders.get("image")->setMat4("projection", glm::ortho(0.f, window.getWidth(), window.getHeight(), 0.f, -1.f, 1.f));
			shaders.get("font")->setMat4("projection", glm::ortho(0.f, window.getWidth(), window.getHeight(), 0.f, -1.f, 1.f));

			_loadingScreen->draw(shaders.get("image"), window.getSize());
			_loadingScreenText->draw(shaders.get("font"), window.getSize());
		}

		void onExit() {}

	private:
		UIElement			*_loadingScreen;
		UIElement			*_loadingScreenText;

		std::atomic<bool>	_loadingDone;
		std::thread			_thread;

		Scene				*_loading;
};

#endif
