/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplayerNewScene.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:01:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/07 12:01:49 by mbatty           ###   ########.fr       */
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

void	MultiplayerNewScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	if (_loaded)
		return ;

	_loaded = true;

	// Decoration of the scene
	_panel.add("background_dark", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5));

	// Cancel button, just goes back to the previous page
	UIElement *tmp = _panel.add("cancel", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"), glm::vec2(0, 84), glm::vec2(0.5, 0.5)));
	_panel.add("cancel_text", new Text("Cancel", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 84), glm::vec2(0.5, 0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new MultiplayerScene(_game));
		});

	// Text field to enter the server's ip
	tmp = _panel.add("ip_text_field", new TextField(textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED), textures.get(TX_PATH_ASCII), shaders.get("image"), shaders.get("font"), glm::vec2(0), glm::vec2(0.5), 24));
	static_cast<TextField*>(tmp)->setClickFunc(
	[this](std::string str)
	{
		this->_ip = str;
	});
	tmp = _panel.add("ip_text_field_text", new Text("Select Adress", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -20), glm::vec2(0.5)));
	static_cast<Text*>(tmp)->setColor(glm::vec3(0.7));

	// Text field to enter the server's name
	tmp = _panel.add("name_text_field", new TextField(textures.get(TX_PATH_TEXT_FIELD), textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED), textures.get(TX_PATH_ASCII), shaders.get("image"), shaders.get("font"), glm::vec2(0, -42), glm::vec2(0.5), 24));
	static_cast<TextField*>(tmp)->setClickFunc(
	[this](std::string str)
	{
		this->_name = str;
	});
	tmp = _panel.add("name_text_field_text", new Text("Server Name", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -62), glm::vec2(0.5)));
	static_cast<Text*>(tmp)->setColor(glm::vec3(0.7));

	// Done text to create a server entry and go back to multiplayer scene
	tmp = _panel.add("done", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"), glm::vec2(0, 62), glm::vec2(0.5, 0.5)));
	_panel.add("done_text", new Text("Done", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 62), glm::vec2(0.5, 0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			// _game->addServerInfo(std::pair<std::string, std::string>(_name, _ip));
			this->_requestScene(new MultiplayerScene(_game));
		});
	static_cast<Button*>(tmp)->setDisabled(true);
}

void	MultiplayerNewScene::onExit()
{
}

void	MultiplayerNewScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	MultiplayerNewScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;

	_panel.handleEvents(events);

	// Only allow creation of a server entry if both text fields have something in them
	if (!_ip.empty() && !_name.empty())
		static_cast<Button*>(_panel.get("done"))->setDisabled(false);
	else
		static_cast<Button*>(_panel.get("done"))->setDisabled(true);
}

void	MultiplayerNewScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
}
