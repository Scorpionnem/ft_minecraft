/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplayerScene.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:45:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 12:55:05 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MultiplayerScene.hpp"
#include "TitleScene.hpp"
#include "Button.hpp"
#include "Game.hpp"
#include "Button.hpp"
#include "Checkbox.hpp"
#include "Image.hpp"
#include "BackgroundImage.hpp"
#include "Text.hpp"
#include "Toggle.hpp"
#include "TextField.hpp"

void	MultiplayerScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();
	
	if (_loaded)
		return ;

	_loaded = true;

	_panel.add("background", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background")));

	UIElement *tmp = _panel.add("back", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), shaders.get("image"),
										glm::vec2(0, -8), glm::vec2(0.5, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new Text("Back", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -14), glm::vec2(0.5, 1.0)));

	_panel.add("ip_text_field", new TextField(textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED), textures.get(TX_PATH_ASCII), shaders.get("image"), shaders.get("font"),
									glm::vec2(0), glm::vec2(0.5), 24));
	_panel.add("ip_text_field_text", new Text("Select IP", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -20), glm::vec2(0.5)));

	_panel.add("connect", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), shaders.get("image"),
										glm::vec2(0, 24), glm::vec2(0.5)));
	_panel.add("connect_text", new Text("Connect", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 24), glm::vec2(0.5)));

	_panel.add("multiplayer_text", new Text("Multiplayer", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 14), glm::vec2(0.5, 0)));
}

void	MultiplayerScene::onExit()
{
}

void	MultiplayerScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	MultiplayerScene::update(float deltaTime)
{
	(void)deltaTime;
	UIEvent	events;

	events.mousePos = _game->getWindow().getMousePos();
	events.windowSize = _game->getWindow().getSize();
	events.inputs = &_game->getInput();

	_panel.handleEvents(events);
}

void	MultiplayerScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
}
