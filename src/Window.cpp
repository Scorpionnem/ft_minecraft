/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:33:19 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 14:31:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Window.hpp"

Window::Window() {}
Window::~Window() {}

void	Window::open()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize glfw");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	this->_width = DEFAULT_WINDOW_WIDTH;
	this->_height = DEFAULT_WINDOW_HEIGHT;
	
	GLFWmonitor	*monitor = NULL;
	#if FULL_SCREEN
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode	*monitorInfos = glfwGetVideoMode(monitor);
		this->_height = monitorInfos->height;
		this->_width = monitorInfos->width;
	#endif
	_data = glfwCreateWindow(this->_width, this->_height, DEFAULT_WINDOW_NAME, monitor, NULL);
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
