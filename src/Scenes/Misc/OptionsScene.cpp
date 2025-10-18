/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionsScene.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 14:04:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OptionsScene.hpp"
#include "TitleScene.hpp"
#include "Game.hpp"
#include "ImprovedBackgroundImage.hpp"
#include "ImprovedButton.hpp"
#include "ImprovedText.hpp"

void	OptionsScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	if (_loaded)
		return ;

	_loaded = true;

	_panel.add("background", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, REFERENCE_HEIGHT), glm::vec2(0), glm::vec2(0), glm::vec2(1), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));

	UIElement *tmp = _panel.add("done", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(0, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));

	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});
	_panel.add("done_text", new ImprovedText("Done", 1, glm::vec2(0.5, 1), glm::vec2(0, -14), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	_panel.add("options_text", new ImprovedText("Options", 1, glm::vec2(0.5, 0), glm::vec2(0, 14), shaders.get("font"), textures.get(TX_PATH_ASCII)));
}

void	OptionsScene::onExit()
{
}

void	OptionsScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	OptionsScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;
	
	_panel.handleEvents(events);
}

void	OptionsScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
}
