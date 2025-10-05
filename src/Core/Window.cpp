/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:33:19 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 11:40:28 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Window.hpp"
#include "Game.hpp"

Window::Window() {}
Window::~Window() {}

void Window::_resize(GLFWwindow *window, int width, int height)
{
	Game	*game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (game)
	{
		game->getWindow().setWidth(width);
		game->getWindow().setHeight(height);
	}
	glViewport(0, 0, width, height);
}

void	Window::_scroll(GLFWwindow *window, double, double yoffset)
{
	Game	*game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (game)
		game->getInput().setScroll(yoffset);
}

void	Window::_keyboardChar(GLFWwindow *window, unsigned int key)
{
	Game	*game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (game)
		game->getInput().addCharInput(key);
}

void	Window::_keyboardInput(GLFWwindow *window, int key, int, int action, int)
{
	Game	*game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	if (game)
	{
		if (action == GLFW_REPEAT)
			game->getInput().setKeyRepeating(key);
	}
}

void	Window::open(const std::string &name, int width, int height, bool fullScreen)
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize glfw");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->_width = width;
	this->_height = height;

	GLFWmonitor	*monitor = NULL;
	if (fullScreen)
	{
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode	*monitorInfos = glfwGetVideoMode(monitor);
		this->_height = monitorInfos->height;
		this->_width = monitorInfos->width;
	}
	_data = glfwCreateWindow(this->_width, this->_height, name.c_str(), monitor, NULL);
	if (!_data)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}
	glfwMakeContextCurrent(_data);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("Failed to init GLAD");
	}
	glViewport(0, 0, this->_width, this->_height);

	glfwSetFramebufferSizeCallback(_data, _resize);
	glfwSetScrollCallback(_data, _scroll);
	glfwSetCharCallback(_data, _keyboardChar);
	glfwSetKeyCallback(_data, _keyboardInput);

	glfwSetCursorPos(_data, _width / 2, _height / 2);

	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void	Window::close()
{
	glfwDestroyWindow(_data);
	glfwTerminate();
}

void	Window::setWindowPointer(void *ptr)
{
	glfwSetWindowUserPointer(_data, ptr);
}

void	Window::frameStart()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_currentFrame = glfwGetTime();
	_deltaTime = _currentFrame - _lastFrame;
	glfwPollEvents();
}

void	Window::frameEnd()
{
	glfwSwapBuffers(_data);

	_lastFrame = _currentFrame;
}
