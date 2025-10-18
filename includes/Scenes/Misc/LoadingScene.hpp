/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoadingScene.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 21:26:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 14:35:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOADINGSCENE_HPP
# define LOADINGSCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"
# include "ImprovedText.hpp"
# include "ImprovedBackgroundImage.hpp"

# include "ShaderManager.hpp"
# include "TextureManager.hpp"
# include "Game.hpp"

constexpr uint FUN_TEXT_COUNT = 11;
constexpr double FUN_TEXT_INTERVAL = 2.5;
constexpr const char	*FUN_TEXT_TITLES[FUN_TEXT_COUNT] =
{
	"Starting game!",
	"Loading textures.",
	"Compiling shaders...",
	"Probably creating the UI elements",
	"The scene should be ready soon",
	"Must be getting closer...",
	"Okay, its taking a bit too much time now!",
	"Hum, where is the scene?",
	"Hey! Please load the scene now!",
	"Bro how long is this going to take?!",
	"This took way too long, I had to stop it from crashing",
};

class LoadingScene : public Scene
{
	public:
		LoadingScene(Game *game, Scene *scene) : Scene(game)
		{
			_id = "LoadingScene";
			_loading = scene;
		}
		~LoadingScene()
		{
			if (!_joined)
				_thread.join();
		};

		void onEnter()
		{
			TextureManager &textures = _game->getTextures();
			ShaderManager &shaders = _game->getShaders();

			Texture	*bgTex;
			if (rand() % 2)
				bgTex = textures.get("assets/textures/fun/mbirou.png");
			else
				bgTex = textures.get("assets/textures/fun/mbatty.png");

			_panel.add("background", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, REFERENCE_HEIGHT), glm::vec2(0), glm::vec2(0), glm::vec2(1), 0.5, shaders.get("background"), bgTex));

			_panel.add("loading_text", new ImprovedText("Loading", 1, glm::vec2(0.5), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));

			ImprovedText *funText = _panel.add("fun_text", new ImprovedText(FUN_TEXT_TITLES[0], 1, glm::vec2(0.5), glm::vec2(0, 13.6), shaders.get("font"), textures.get(TX_PATH_ASCII)));
			funText->setColor(glm::vec4(0.7, 0.7, 0.7, 1.0));
			// funText->setScale(0.7);

			_loadingDone = false;
			_thread = std::thread([this]()
				{
					_loading->onEnter();

					_loadingDone = true;
				});
		}

		void processInput(float ) {}
		void update(UIEvent, float)
		{
			if (_loadingDone)
			{
				_thread.join();
				_joined = true;
				_requestScene(_loading);
				return ;
			}

			ImprovedText	*loadingScreenText = static_cast<ImprovedText*>(_panel.get("loading_text"));
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

			ImprovedText *funText = static_cast<ImprovedText*>(_panel.get("fun_text"));
			if (glfwGetTime() - _lastFunTextUpdate > FUN_TEXT_INTERVAL)
			{
				if (_FunTextIt + 1 <= FUN_TEXT_COUNT)
					funText->setText(FUN_TEXT_TITLES[_FunTextIt++]);

				_lastFunTextUpdate = glfwGetTime();
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
		double				_lastFunTextUpdate = 0;
		uint				_FunTextIt = 1;
		bool				_joined = false;
		Panel				_panel;

		std::atomic<bool>	_loadingDone;
		std::thread			_thread;

		Scene				*_loading;
};

#endif
