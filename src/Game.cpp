/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:28:00 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 20:03:57 by mbatty           ###   ########.fr       */
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
	_currentScene->onExit();
	delete _currentScene;

	_currentScene = scene;
	if (!_currentScene->loaded())
		_currentScene->onEnter();
}

void	Game::_init()
{
	srand(std::time(NULL));

	_window.open(WINDOW_NAME, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, false);
	_window.setWindowPointer(this);

	_loadTextures();
	_loadShaders();

	_currentScene = new LoadingScene(this, new TitleScene(this));
	_currentScene->onEnter();
}

void	Game::_loadTextures()
{
	_textures.load("missing");

	_textures.load("ascii", "assets/textures/ui/font/ascii.png");
	_textures.load("dirt", "assets/textures/block/dirt.png");
	_textures.load("ft_minecraft", "assets/textures/ui/ft_minecraft.png");

	_textures.load("button", "assets/textures/ui/button.png");
	_textures.load("button_highlighted", "assets/textures/ui/button_highlighted.png");
	_textures.load("button_small", "assets/textures/ui/button_small.png");
	_textures.load("button_highlighted_small", "assets/textures/ui/button_highlighted_small.png");

	_textures.load("slider_handle_highlighted", "assets/textures/ui/slider_handle_highlighted.png");
	_textures.load("slider_handle", "assets/textures/ui/slider_handle.png");

	_textures.load("text_field", "assets/textures/ui/text_field.png");
	_textures.load("text_field_highlighted", "assets/textures/ui/text_field_highlighted.png");

	_textures.load("hotbar", "assets/textures/ui/hud/hotbar.png");
	_textures.load("hotbar_slot", "assets/textures/ui/hud/hotbar_selection.png");
}

void	Game::_loadShaders()
{
	_shaders.load("font", "shaders/text.vertex", "shaders/text.fragment");
	_shaders.load("image", "shaders/image.vertex", "shaders/image.fragment");
	_shaders.load("background", "shaders/background.vertex", "shaders/background.fragment");
}

void	Game::_stop()
{
	if (_currentScene)
	{
		_currentScene->onExit();
		delete _currentScene;
	}
	_window.close();
}

void	Game::_processInput()
{
	_input.resetCharInputs();
	_input.resetKeyRepeats();

	_window.frameStart();

	_input.update(_window.data());

	// Hard exit
	if (_input.isKeyDown(GLFW_KEY_ESCAPE) && _input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		_running = false;

	_currentScene->processInput(_window.getDeltaTime());
}

void	Game::_update(float deltaTime)
{
	if (!_window.up())
		_running = false;

	_currentScene->update(deltaTime);
}

void	Game::_render()
{
	_currentScene->render();

	_window.frameEnd();
}
