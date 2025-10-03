/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplayerNewScene.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:01:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 16:52:36 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MultiplayerScene.hpp"
#include "MultiplayerNewScene.hpp"

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
#include "ScaledBackgroundImage.hpp"

	// _panel.add("ip_text_field", new TextField(textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED), textures.get(TX_PATH_ASCII), shaders.get("image"), shaders.get("font"),
	// 								glm::vec2(0), glm::vec2(0.5), 24));
	// _panel.add("ip_text_field_text", new Text("Select IP", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -20), glm::vec2(0.5)));

	// _panel.add("connect", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), shaders.get("image"),
	// 									glm::vec2(0, 24), glm::vec2(0.5)));
	// _panel.add("connect_text", new Text("Connect", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 24), glm::vec2(0.5)));

void	MultiplayerNewScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();
	
	if (_loaded)
		return ;

	_loaded = true;
	
	_panel.add("background_dark", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5));

	UIElement *tmp = _panel.add("done", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(0, 62), glm::vec2(0.5, 0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new MultiplayerScene(_game));
		});
	static_cast<Button*>(tmp)->setDisabled(true);
	tmp = _panel.add("done_text", new Text("Done", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 62), glm::vec2(0.5, 0.5)));

	tmp = _panel.add("cancel", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(0, 84), glm::vec2(0.5, 0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new MultiplayerScene(_game));
		});
	_panel.add("cancel_text", new Text("Cancel", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 84), glm::vec2(0.5, 0.5)));

	_panel.add("ip_text_field", new TextField(textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED), textures.get(TX_PATH_ASCII), shaders.get("image"), shaders.get("font"),
									glm::vec2(0), glm::vec2(0.5), 24));
	tmp = _panel.add("ip_text_field_text", new Text("Select Adress", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -20), glm::vec2(0.5)));
	static_cast<Text*>(tmp)->setColor(glm::vec3(0.7));
	
	_panel.add("name_text_field", new TextField(textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED), textures.get(TX_PATH_ASCII), shaders.get("image"), shaders.get("font"),
									glm::vec2(0, -42), glm::vec2(0.5), 24));
	tmp = _panel.add("name_text_field_text", new Text("Server Name", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -62), glm::vec2(0.5)));
	static_cast<Text*>(tmp)->setColor(glm::vec3(0.7));
}

void	MultiplayerNewScene::onExit()
{
}

void	MultiplayerNewScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	MultiplayerNewScene::update(float deltaTime)
{
	(void)deltaTime;
	UIEvent	events;

	events.mousePos = _game->getWindow().getMousePos();
	events.windowSize = _game->getWindow().getSize();
	events.inputs = &_game->getInput();

	_panel.handleEvents(events);
}

void	MultiplayerNewScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
}
