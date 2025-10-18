/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorScene.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:56:40 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 11:52:50 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORSCENE_HPP
# define ERRORSCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"
# include "ImprovedText.hpp"
# include "ImprovedBackgroundImage.hpp"
# include "ImprovedButton.hpp"

# include "ShaderManager.hpp"
# include "TextureManager.hpp"
# include "Game.hpp"
# include "Panel.hpp"
# include "TitleScene.hpp"

/*
	Use this scene to print an exception throwed by the program (In a scene _requestScene this scene when you catch a fatal error (But not fatal enough to stop the program))
*/
class ErrorScene : public Scene
{
	public:
		ErrorScene(Game *game, const std::string &error) : Scene(game)
		{
			_id = "ErrorScene";
			_error = "An error occured: " + error;
		}
		~ErrorScene()
		{
		};

		void onEnter()
		{
			TextureManager &textures = _game->getTextures();
			ShaderManager &shaders = _game->getShaders();

			_panel.add("background", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, REFERENCE_HEIGHT), glm::vec2(0), glm::vec2(0), glm::vec2(1), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));

			_panel.add("loading_text", new ImprovedText(_error, 1, glm::vec2(0.5), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));

			UIElement *tmp = _panel.add("back", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(0, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
			static_cast<ImprovedButton*>(tmp)->setClickFunc(
				[this]()
				{
					this->_requestScene(new TitleScene(_game));
				});
			_panel.add("back_text", new ImprovedText("Back", 1, glm::vec2(0.5, 1.0), glm::vec2(0, -14), shaders.get("font"), textures.get(TX_PATH_ASCII)));
		}

		void processInput(float ) {}
		void update(UIEvent events, float)
		{
			_panel.handleEvents(events);
		}
		void render()
		{
			Window	&window = _game->getWindow();

			_panel.draw(window.getSize());
		}

		void onExit() {}

	private:
		std::string			_error;
		Panel				_panel;
};

#endif
