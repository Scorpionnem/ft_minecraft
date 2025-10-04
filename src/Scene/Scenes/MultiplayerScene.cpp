/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplayerScene.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:45:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/04 11:16:33 by mbatty           ###   ########.fr       */
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

void	MultiplayerScene::refreshServerList()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();
	std::vector<ServerInfo>	&serverList = _game->getServerInfos();
	_serversPanel.clear();
	float	yOffset = 42.5;
	int	i = 0;

	_serversPanel.add("background_dark", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.1875));

	for (ServerInfo &server : serverList)
	{
		UIElement	*tmp = _serversPanel.add("server_button_" + std::to_string(i), new Checkbox(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), shaders.get("image"),
										glm::vec2(0, yOffset), glm::vec2(0.5, 0)));
		static_cast<Checkbox*>(tmp)->setClickFunc(
			[this, &server](bool state)
			{
				if (state)
					this->_game->setCurrentServer(&server);
				else
					this->_game->setCurrentServer(NULL);
			});
		_serversPanel.add("server_text_" + std::to_string(i), new LimitedText(server.name, textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, yOffset + 6), glm::vec2(0.5, 0), 24));
		i++;
		yOffset += 22;
	}
}

void	MultiplayerScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();
	
	refreshServerList();

	if (_loaded)
		return ;

	_loaded = true;
	
	_panel.add("background1", new ScaledBackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5, glm::vec2(0), glm::vec2(0, 1), glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT * 0.0875)));
	_panel.add("background1", new ScaledBackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5, glm::vec2(0), glm::vec2(0, 0), glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT * 0.05)));

	UIElement *tmp = _panel.add("back", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(102, -8), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new Text("Back", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(102, -14), glm::vec2(0.5, 1.0)));

	tmp = _panel.add("create_server", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(102, -30), glm::vec2(0.5, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new MultiplayerNewScene(_game));
		});

	_panel.add("create_server_text", new Text("Add Server", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(102, -36), glm::vec2(0.5, 1.0)));
	
	tmp = _panel.add("play_server", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(-102, -30), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	static_cast<Button*>(tmp)->setDisabled(true);

	_panel.add("play_server_text", new Text("Join Server", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-102, -36), glm::vec2(0.5, 1.0)));
	
	tmp = _panel.add("edit_server", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED), shaders.get("image"),
										glm::vec2(-153, -8), glm::vec2(0.5, 1), glm::vec2(0.98, 1)));
	static_cast<Button*>(tmp)->setDisabled(true);
	_panel.add("edit_server_text", new Text("Edit", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-153, -14), glm::vec2(0.5, 1.0)));

	tmp = _panel.add("delete_server", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED), shaders.get("image"),
										glm::vec2(-51, -8), glm::vec2(0.5, 1), glm::vec2(0.98, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			_game->deleteServer(_game->getCurrentServer());
			this->refreshServerList();
		});
	static_cast<Button*>(tmp)->setDisabled(true);
	_panel.add("delete_server_text", new Text("Delete", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-51, -14), glm::vec2(0.5, 1.0)));

	_panel.add("multiplayer_text", new Text("Multiplayer", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 14), glm::vec2(0.5, 0)));

	refreshServerList();
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

	if (_game->getCurrentServer())
	{
		static_cast<Button*>(_panel.get("delete_server"))->setDisabled(false);
		static_cast<Button*>(_panel.get("play_server"))->setDisabled(false);
		static_cast<Button*>(_panel.get("edit_server"))->setDisabled(false);
	}
	else
	{
		static_cast<Button*>(_panel.get("delete_server"))->setDisabled(true);
		static_cast<Button*>(_panel.get("play_server"))->setDisabled(true);
		static_cast<Button*>(_panel.get("edit_server"))->setDisabled(true);
	}

	_panel.handleEvents(events);
	_serversPanel.handleEvents(events);
}

void	MultiplayerScene::render()
{
	Window	&window = _game->getWindow();

	_serversPanel.draw(window.getSize());
	_panel.draw(window.getSize());
}
