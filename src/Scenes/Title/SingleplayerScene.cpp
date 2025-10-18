/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleplayerScene.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:45:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 13:11:01 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SingleplayerScene.hpp"
#include "TitleScene.hpp"
#include "Game.hpp"
#include "GameScene.hpp"

#include "ImprovedBackgroundImage.hpp"
#include "ImprovedButton.hpp"
#include "ImprovedText.hpp"

void	SingleplayerScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	if (_loaded)
		return ;

	_loaded = true;

	_panel.add("background_dark", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, REFERENCE_HEIGHT), glm::vec2(0), glm::vec2(0), glm::vec2(1), 0.1875, shaders.get("background"), textures.get(TX_PATH_DIRT)));

	_panel.add("background1", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, 58), glm::vec2(0, 1), glm::vec2(0), glm::vec2(1, 0), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));
	_panel.add("background2", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, 29), glm::vec2(0, 0), glm::vec2(0), glm::vec2(1, 0), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));

	_panel.add("singleplayer_text", new ImprovedText("Singleplayer", 1, glm::vec2(0.5, 0.0), glm::vec2(0, 14), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	UIElement *tmp = _panel.add("back", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(102, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new ImprovedText("Back", 1, glm::vec2(0.5, 1.0), glm::vec2(102, -14), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	tmp = _panel.add("create_world", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(102, -30), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
	[this]()
	{
		this->_requestScene(new GameScene(_game));
	});
	
	_panel.add("create_world_text", new ImprovedText("Create New World", 1, glm::vec2(0.5, 1.0), glm::vec2(102, -36), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	tmp = _panel.add("play_world", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(-102, -30), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->disable();
	_panel.add("play_world_text", new ImprovedText("Play Selected World", 1, glm::vec2(0.5, 1.0), glm::vec2(-102, -36), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	
	tmp = _panel.add("edit_world", new ImprovedButton(glm::vec2(98, 20), glm::vec2(0.5, 1), glm::vec2(-153, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->disable();
	_panel.add("edit_world_text", new ImprovedText("Edit", 1, glm::vec2(0.5, 1.0), glm::vec2(-153, -14), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	tmp = _panel.add("delete_world", new ImprovedButton(glm::vec2(98, 20), glm::vec2(0.5, 1), glm::vec2(-51, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->disable();
	_panel.add("delete_world_text", new ImprovedText("Delete", 1, glm::vec2(0.5, 1.0), glm::vec2(-51, -14), shaders.get("font"), textures.get(TX_PATH_ASCII)));
}

void	SingleplayerScene::onExit()
{
}

void	SingleplayerScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	SingleplayerScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;

	_worldsPanel.handleEvents(events);
	_panel.handleEvents(events);
}

void	SingleplayerScene::render()
{
	Window	&window = _game->getWindow();

	_worldsPanel.draw(window.getSize());
	_panel.draw(window.getSize());
}
