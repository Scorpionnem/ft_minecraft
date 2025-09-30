/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:28:00 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 02:04:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

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
	}
	_stop();
}

void	Game::_init()
{
	_window.open(WINDOW_NAME, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, false);
	_window.setWindowPointer(this);

	// These will be used in the "loading screen" so they HAVE to be loaded
	_shaders.load("font", "shaders/text.vertex", "shaders/text.fragment");
	_shaders.get("font")->setInt("tex0", 0);
	_textures.load("ascii", "assets/textures/ui/font/ascii.png");
	_textures.load("ft_minecraft", "assets/textures/ui/ft_minecraft.png");
	_shaders.load("image", "shaders/image.vertex", "shaders/image.fragment");

	_loadTextures();
	_loadShaders();
}

void	Game::_loadTextures()
{
}

void	Game::_loadShaders()
{
}

void	Game::_stop()
{
	_window.close();
}

void	Game::_processInput()
{
	_window.frameStart();

	_input.update(_window.data());

	if (_input.isKeyPressed(GLFW_KEY_ESCAPE))
		_running = false;
}

void	Game::_updateCamera(float deltaTime)
{
	float	cameraSpeed = 1 * deltaTime;
	float	speedBoost = 1.0f;

	if (_input.isKeyDown(GLFW_KEY_LEFT_CONTROL))
		speedBoost = 20.0f;

	if (_input.isKeyDown(GLFW_KEY_LEFT_ALT))
		speedBoost *= 15.0f;

	if (_input.isKeyDown(GLFW_KEY_W))
		_camera.setPos(_camera.getPos() + _camera.getFront() * (cameraSpeed * speedBoost));
	if (_input.isKeyDown(GLFW_KEY_S))
		_camera.setPos(_camera.getPos() - _camera.getFront() * (cameraSpeed * speedBoost));
	if (_input.isKeyDown(GLFW_KEY_SPACE))
		_camera.setPos(_camera.getPos() + _camera.getUp() * (cameraSpeed * speedBoost));
	if (_input.isKeyDown(GLFW_KEY_LEFT_SHIFT))
		_camera.setPos(_camera.getPos() - _camera.getUp() * (cameraSpeed * speedBoost));

	if (_input.isKeyDown(GLFW_KEY_A))
		_camera.setPos(_camera.getPos() - glm::normalize(glm::cross(_camera.getFront(), _camera.getUp())) * (cameraSpeed * speedBoost));
	if (_input.isKeyDown(GLFW_KEY_D))
		_camera.setPos(_camera.getPos() + glm::normalize(glm::cross(_camera.getFront(), _camera.getUp())) * (cameraSpeed * speedBoost));
}

void	Game::_update(float deltaTime)
{
	if (!_window.up())
		_running = false;

	_updateCamera(deltaTime);
}

#include "Image.hpp"

constexpr float REFERENCE_WIDTH = DEFAULT_WINDOW_WIDTH * 2;
constexpr float REFERENCE_HEIGHT = DEFAULT_WINDOW_HEIGHT * 2;

float	getUiScale(Window &window)
{
	float windowWidth  = (float)window.getWidth();
	float windowHeight = (float)window.getHeight();

	float scaleX = windowWidth / REFERENCE_WIDTH;
	float scaleY = windowHeight / REFERENCE_HEIGHT;

	return (std::min(scaleX, scaleY));
}

void	Game::_render()
{
	Image	image(_textures.get("ft_minecraft"));

	_shaders.get("image")->setMat4("projection", glm::ortho(0.f, _window.getWidth(), _window.getHeight(), 0.f, -1.f, 1.f));

	float	uiScale = getUiScale(_window);

	image.draw(glm::vec2((_window.getWidth() / 2) - (image.getWidth(uiScale) / 2), 48 * uiScale), _shaders.get("image"), uiScale);

	glDisable(GL_DEPTH_TEST);

	_window.frameEnd();
}
