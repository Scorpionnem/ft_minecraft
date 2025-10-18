/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:28:00 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 11:17:13 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "TitleScene.hpp"
#include "OptionsScene.hpp"
#include "GameScene.hpp"
#include "LoadingScene.hpp"
#include "ErrorScene.hpp"

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

	try {
		_currentScene->onEnter();
	} catch (const std::exception &e) {
		if (_currentScene->id() != "ErrorScene")
			_swapScene(new ErrorScene(this, e.what()));
		else
			throw ;
	}
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

int	Game::_processCharMaxWidth(Texture *font, int start_width, int start_height, int char_width, int char_height)
{
	int		maxX = 0;
	auto	pixel = font->getPixels();
	
	for (int x = start_width; x < start_width + char_width; x++)
	{
		for (int y = (font->getHeight() - start_height - char_height); y < (font->getHeight() - start_height - char_height) + char_height; y++)
		{
			int	alpha = (int)pixel[(x + y * font->getWidth()) * 4 + 3];
			if (alpha != 0)
				maxX = x - start_width;
		}
	}
	return (maxX + 1);
}

std::map<char, int>	_charSpacing;

void	Game::_processFontSpacing(Texture *font)
{
	int	img_height = font->getHeight();
	int	img_width = font->getWidth();

	int	char_height = img_height / 16;
	int	char_width = img_width / 16;

	if (img_height / char_height != 16 || img_width / char_width != 16 || font->getFormat() != GL_RGBA)
	{
		std::cout << "Invalid font format" << std::endl;
		return ;
	}

	char	c = 0;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			int t = _processCharMaxWidth(font, x * char_width, y * char_height, char_width, char_height);
			_charSpacing[c] = t;
			c++;
		}
	}
	_charSpacing[' '] = 8;
	_charSpacing['\t'] = 8;
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

	_processFontSpacing(_textures.get(TX_PATH_ASCII));

	_textures.upload();
}

void	Game::_loadShaders()
{
	_shaders.load("font", "assets/shaders/text.vert", "assets/shaders/text.frag");
	_shaders.load("image", "assets/shaders/image.vert", "assets/shaders/image.frag");
	_shaders.load("background", "assets/shaders/background.vert", "assets/shaders/background.frag");
	_shaders.load("quad", "assets/shaders/quad.vert", "assets/shaders/quad.frag");
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
	std::ofstream	file(SERVER_LIST_EXPORT_FILE);

	if (!file.is_open())
		return ;

	nlohmann::json	data;
	data["servers"] = _servers;

	file << std::setw(4) << data << std::endl;
}

bool	Game::isValidServer(const json &server)
{
	if (!server.contains("name") || !server.contains("ip") || !server.contains("port"))
		return (false);

	if (!server["name"].is_string())
		return (false);
	if (!server["ip"].is_string())
		return (false);
	if (!server["port"].is_number())
		return (false);

	return (true);
}

void	Game::importServerList()
{
	std::ifstream	file(SERVER_LIST_EXPORT_FILE);

	if (!file.is_open())
		return ;

	nlohmann::json	data;

	try {
		data = json::parse(file);
	} catch (const std::exception &e) {
		std::cerr << "Failed to parse " << SERVER_LIST_EXPORT_FILE << "\n" << e.what() << std::endl;
	}

	std::vector<json>	validServers;

	if (data.contains("servers") && data["servers"].is_array())
		validServers = data["servers"];
	else
		std::cerr << "Invalid format in " << SERVER_LIST_EXPORT_FILE << std::endl;

	for (const json &server : validServers)
	{
		if (isValidServer(server))
			_servers.push_back(server);
		else
			std::cerr << "Invalid server found in file " << SERVER_LIST_EXPORT_FILE << std::endl;
	}
}
