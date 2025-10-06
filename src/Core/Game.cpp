/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:28:00 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/06 11:17:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Button.hpp"
#include "TitleScene.hpp"
#include "OptionsScene.hpp"
#include "GameScene.hpp"
#include "LoadingScene.hpp"

Game::Game() {}
Game::~Game() {}

void	Game::run()
{
	_init();
	while (_running)
	{
		_textures.upload();

		_processInput();
		_update(_window.getDeltaTime());
		_render();

		if (_currentScene->requestedScene())
			_swapScene(_currentScene->getSceneRequest());
	}
	_stop();
}

void	Game::_swapScene(Scene *scene)
{
	std::string	oldId = _currentScene->id();
	_currentScene->resetRequest();
	_currentScene->onExit();

	if (_currentScene->keepAlive())
	{
		if (_scenes.find(_currentScene->id()) == _scenes.end())
			_scenes.insert({_currentScene->id(), _currentScene});
	}
	else
	{
		if (_scenes.find(_currentScene->id()) != _scenes.end())
			_scenes.erase(_scenes.find(_currentScene->id()));
		delete _currentScene;
	}

	if (_scenes.find(scene->id()) != _scenes.end())
	{
		_currentScene = _scenes.find(scene->id())->second;
		delete scene;
	}
	else
	{
		_currentScene = scene;
		_scenes.insert({_currentScene->id(), _currentScene});
	}

	_currentScene->onEnter();
}

void	Game::_init()
{
	srand(std::time(NULL));

	std::filesystem::create_directory("saves");

	_window.open(WINDOW_NAME, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, false);
	_window.setWindowPointer(this);

	_loadTextures();
	_loadShaders();

	importServerList();

	_currentScene = new LoadingScene(this, new TitleScene(this));
	_scenes.insert({_currentScene->id(), _currentScene});
	_currentScene->onEnter();
}

void	Game::_loadTextures()
{
	/*

		Load textures that always need to be loaded!

	*/
	_textures.get(TX_PATH_ASCII);
	_textures.get(TX_PATH_DIRT);
	_textures.get(TX_PATH_FT_MINECRAFT);

	_textures.get(TX_PATH_BUTTON);
	_textures.get(TX_PATH_BUTTON_HIGHLIGHTED);
	_textures.get(TX_PATH_BUTTON_SMALL);
	_textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED);

	_textures.get(TX_PATH_SLIDER_HANDLE_HIGHLIGHTED);
	_textures.get(TX_PATH_SLIDER_HANDLE);

	_textures.get(TX_PATH_TEXT_FIELD);
	_textures.get(TX_PATH_TEXT_FIELD_HIGHLIGHTED);

	_textures.upload();
}

void	Game::_loadShaders()
{
	_shaders.load("font", "assets/shaders/text.vert", "assets/shaders/text.frag");
	_shaders.load("image", "assets/shaders/image.vert", "assets/shaders/image.frag");
	_shaders.load("background", "assets/shaders/background.vert", "assets/shaders/background.frag");
}

void	Game::_stop()
{
	for (auto &scene : _scenes)
	{
		scene.second->onExit();
		delete scene.second;
	}
	exportServerList();
	_window.close();
}

void	Game::_processInput()
{
	_input.reset();

	_window.frameStart();

	_input.update(_window.data());

	// Hard exit
	if (_input.isKeyDown(GLFW_KEY_ESCAPE) && _input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		_running = false;
	if (_input.isKeyPressed(GLFW_KEY_R) && _input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		_shaders.reload();

	_currentScene->processInput(_window.getDeltaTime());
}

void	Game::_update(float deltaTime)
{
	if (!_window.up())
		_running = false;

	UIEvent	events;

	events.mousePos = _window.getMousePos();
	events.windowSize = _window.getSize();
	events.inputs = &_input;

	_currentScene->update(events, deltaTime);
}

void	Game::_render()
{
	_currentScene->render();

	_window.frameEnd();
}

void	Game::exportServerList()
{
	std::ofstream	file;

	file.open(SERVER_LIST_EXPORT_FILE);
	if (!file.is_open())
		return ;

	for (ServerInfo &server : _serverInfos)
		file << server.name << " " << server.ip << std::endl;
}

void	Game::importServerList()
{
	std::ifstream	file;
	std::string		fileLine;

	file.open(SERVER_LIST_EXPORT_FILE);
	if (!file.is_open())
		return ;

	while (std::getline(file, fileLine))
	{
		ServerInfo			server;
		std::istringstream	line(fileLine);

		if (line >> server.name >> server.ip)
			_serverInfos.push_back(server);
	}
}

void	Game::deleteServer(ServerInfo *info)
{
	_currentServer = NULL;
	for (std::vector<ServerInfo>::iterator server = _serverInfos.begin(); server != _serverInfos.end(); server++)
	{
		if (&*server == info)
		{
			server = _serverInfos.erase(server);
			return ;
		}
	}
}
