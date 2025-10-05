/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleplayerScene.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:45:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 12:26:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SingleplayerScene.hpp"
#include "TitleScene.hpp"
#include "Button.hpp"
#include "Game.hpp"
#include "Button.hpp"
#include "Checkbox.hpp"
#include "Image.hpp"
#include "BackgroundImage.hpp"
#include "ScaledBackgroundImage.hpp"
#include "Text.hpp"
#include "Toggle.hpp"
#include "TextField.hpp"

void	SingleplayerScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	if (_loaded)
		return ;

	_loaded = true;

	_panel.add("background_dark", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.1875));

	_panel.add("background1", new ScaledBackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5, glm::vec2(0), glm::vec2(0, 1), glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT * 0.0875)));
	_panel.add("background1", new ScaledBackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5, glm::vec2(0), glm::vec2(0, 0), glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT * 0.05)));

	UIElement *tmp = _panel.add("back", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(102, -8), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new Text("Back", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(102, -14), glm::vec2(0.5, 1.0)));

	tmp = _panel.add("create_world", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(102, -30), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	_panel.add("create_world_text", new Text("Create New World", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(102, -36), glm::vec2(0.5, 1.0)));

	tmp = _panel.add("play_world", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(-102, -30), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	static_cast<Button*>(tmp)->setDisabled(true);

	_panel.add("play_world_text", new Text("Play Selected World", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-102, -36), glm::vec2(0.5, 1.0)));


	tmp = _panel.add("edit_world", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED), shaders.get("image"),
										glm::vec2(-153, -8), glm::vec2(0.5, 1), glm::vec2(0.98, 1)));
	static_cast<Button*>(tmp)->setDisabled(true);

	_panel.add("edit_world_text", new Text("Edit", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-153, -14), glm::vec2(0.5, 1.0)));

	tmp = _panel.add("delete_world", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED), shaders.get("image"),
										glm::vec2(-51, -8), glm::vec2(0.5, 1), glm::vec2(0.98, 1)));
	static_cast<Button*>(tmp)->setDisabled(true);

	_panel.add("delete_world_text", new Text("Delete", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-51, -14), glm::vec2(0.5, 1.0)));

	_panel.add("singleplayer_text", new Text("Singleplayer", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 14), glm::vec2(0.5, 0)));
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
