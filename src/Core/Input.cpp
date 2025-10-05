/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:18:28 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 11:37:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Input.hpp"

void	Input::update(GLFWwindow *window)
{
	_previous = _current;
	_previousMouse = _currentMouse;

	for (int i = 0; i < MAX_KEYS; ++i)
		_current[i] = glfwGetKey(window, i) == GLFW_PRESS;

	for (int i = 0; i < MAX_MOUSE_KEYS; ++i)
		_currentMouse[i] = glfwGetMouseButton(window, i) == GLFW_PRESS;
}

bool	Input::isMouseDown(int key) const
{
	return (_currentMouse[key]);
}

bool	Input::isKeyDown(int key) const
{
	return (_current[key]);
}

bool	Input::isKeyPressed(int key) const
{
	return (_current[key] && !_previous[key]);
}

bool	Input::isKeyReleased(int key) const
{
	return (!_current[key] && _previous[key]);
}

bool	Input::isMousePressed(int key) const
{
	return (_currentMouse[key] && !_previousMouse[key]);
}

bool	Input::isMouseReleased(int key) const
{
	return (!_currentMouse[key] && _previousMouse[key]);
}

bool	Input::isKeyRepeating(int key) const
{
	return (_repeats[key]);
}
