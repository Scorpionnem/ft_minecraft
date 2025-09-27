/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:18:28 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 13:21:01 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Input.hpp"

void	Input::update(GLFWwindow *window)
{
	_previous = _current;

	for (int i = 0; i < MAX_KEYS; ++i)
		_current[i] = glfwGetKey(window, i) == GLFW_PRESS;
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
