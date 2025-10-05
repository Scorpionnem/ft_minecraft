/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplayerScene.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:45:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 12:26:48 by mbatty           ###   ########.fr       */
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

# define MAX_SERVERS_COUNT 10
# define SERVER_LIST_START_OFFSET 42.5
# define SERVER_LIST_BUTTON_OFFSET 22

void	MultiplayerScene::refreshServerList()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();
	std::vector<ServerInfo>	&serverList = _game->getServerInfos();
	_serversPanel.clear();
	float	yOffset = SERVER_LIST_START_OFFSET;
	int	i = 0;

	// Decoration of the panel
	_serversPanel.add("background_dark", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.1875));

	// For each entry in the server list, we create a Checkbox with the server's name.
	for (ServerInfo &server : serverList)
	{
		if (i >= MAX_SERVERS_COUNT)
			break ;

		UIElement	*tmp = _serversPanel.add("server_button_" + std::to_string(i), new Checkbox(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), shaders.get("image"),
										glm::vec2(0, yOffset), glm::vec2(0.5, 0)));
		_serversPanel.add("server_text_" + std::to_string(i), new LimitedText(server.name, textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, yOffset + 6), glm::vec2(0.5, 0), 24));
		static_cast<Checkbox*>(tmp)->setClickFunc(
			[this, &server](bool state)
			{
				if (state)
					this->_game->setCurrentServer(&server);
				else
					this->_game->setCurrentServer(NULL);
			});
		i++;
		yOffset += SERVER_LIST_BUTTON_OFFSET;
	}
}

#include "ImprovedBackgroundImage.hpp"

void	MultiplayerScene::createPanels()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	// Background/Decoration of the scene
	_panel.add("background1", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, 58), glm::vec2(0, 1), glm::vec2(0), glm::vec2(1, 0), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));
	_panel.add("background2", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, 29), glm::vec2(0, 0), glm::vec2(0), glm::vec2(1, 0), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));

	_panel.add("multiplayer_text", new Text("Multiplayer", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 14), glm::vec2(0.5, 0)));

	// Back button to go back to the TitleScene
	UIElement *tmp = _panel.add("back", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"), glm::vec2(102, -8), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	_panel.add("back_text", new Text("Back", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(102, -14), glm::vec2(0.5, 1.0)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});

	createServerHandling();
}

void	MultiplayerScene::createServerHandling()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	// Create server button (Will switch to MultiPLayerScene that will create a new server entry)
	UIElement	*tmp = _panel.add("create_server", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"), glm::vec2(102, -30), glm::vec2(0.5, 1)));
	_panel.add("create_server_text", new Text("Add Server", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(102, -36), glm::vec2(0.5, 1.0)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new MultiplayerNewScene(_game));
		});

	// Play selected server button
	tmp = _panel.add("play_server", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"), glm::vec2(-102, -30), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	_panel.add("play_server_text", new Text("Join Server", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-102, -36), glm::vec2(0.5, 1.0)));
	static_cast<Button*>(tmp)->setDisabled(true);

	// Edit selected server
	tmp = _panel.add("edit_server", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED), shaders.get("image"), glm::vec2(-153, -8), glm::vec2(0.5, 1), glm::vec2(0.98, 1)));
	_panel.add("edit_server_text", new Text("Edit", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-153, -14), glm::vec2(0.5, 1.0)));
	static_cast<Button*>(tmp)->setDisabled(true);

	// Delete selected server
	tmp = _panel.add("delete_server", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED), shaders.get("image"), glm::vec2(-51, -8), glm::vec2(0.5, 1), glm::vec2(0.98, 1)));
	_panel.add("delete_server_text", new Text("Delete", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-51, -14), glm::vec2(0.5, 1.0)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			_game->deleteServer(_game->getCurrentServer());
			this->refreshServerList();
		});
	static_cast<Button*>(tmp)->setDisabled(true);
}

void	MultiplayerScene::onEnter()
{
	_game->setCurrentServer(NULL);
	refreshServerList();

	if (_loaded)
		return ;

	_loaded = true;

	createPanels();

	refreshServerList();
}

void	MultiplayerScene::onExit()
{
}

void	MultiplayerScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	MultiplayerScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;

	// Activate / Deactivate selected server handling buttons
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

	// If too many servers, lock the create button (Ideally I have to add scroll functionnality)
	if (_game->getServerInfos().size() >= MAX_SERVERS_COUNT)
		static_cast<Button*>(_panel.get("create_server"))->setDisabled(true);
	else
		static_cast<Button*>(_panel.get("create_server"))->setDisabled(false);

	_panel.handleEvents(events);
	_serversPanel.handleEvents(events);
}

void	MultiplayerScene::render()
{
	Window	&window = _game->getWindow();

	_serversPanel.draw(window.getSize());
	_panel.draw(window.getSize());
}
