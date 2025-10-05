/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:52:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 19:29:01 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

void	Camera::update(float deltaTime)
{
	_direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_direction.y = sin(glm::radians(_pitch));
	_direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(_direction);
	_right = glm::normalize(glm::cross(_front, _up));
	_up = glm::normalize(glm::cross(_right, _front));

	_speed = glm::length(_pos - _lastPos) / deltaTime;
	_lastPos = _pos;
}

void	Camera::setViewProjectionMatrix(Shader *shader, float fov, float aspectRatio, float farPlane)
{
	glm::mat4	view = this->getViewMatrix();
	glm::mat4	projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, farPlane);

	shader->bind();
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
}

glm::mat4	Camera::getViewMatrix()
{
	return (glm::lookAt(_pos, _pos + _front, _up));
}
