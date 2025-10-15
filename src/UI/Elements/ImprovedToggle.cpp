/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedToggle.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 08:47:19 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/15 08:56:06 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ImprovedToggle.hpp"

ImprovedToggle::ImprovedToggle(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *shader, Texture *defaultTexture, Texture *hoverTexture, Texture *disabledTexture)
{
	_size = size;
	_anchor = anchor;
	_offset = offset;
	_shader = shader;
	_defaultTexture = defaultTexture;
	_hoverTexture = hoverTexture;
	_disabledTexture = disabledTexture;
}

void	ImprovedToggle::handleEvents(UIEvent events)
{
	if (_disabled)
	{
		_activeTexture = _disabledTexture;
		return ;
	}

	_scale = UIElement::_getUiScale(events.windowSize);
	_pos = UIElement::_getScaledPos(_size, _anchor, _offset, events.windowSize);
	bool		inside = _isInBounds(events.mousePos, _pos, this->_size * _scale);

	if (inside || _clicked)
		_activeTexture = _hoverTexture;
	else
		_activeTexture = _defaultTexture;

	if (events.inputs->isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (inside)
			_clicked = !_clicked;
		else
			_clicked = false;
		if (_onClick)
			_onClick(_clicked);
	}
}

void	ImprovedToggle::draw(glm::vec2 windowSize)
{
	if (!_activeTexture || !_shader)
		return ;

	_upload();

	_scale = UIElement::_getUiScale(windowSize);
	_pos = UIElement::_getScaledPos(_size, _anchor, _offset, windowSize);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(_pos, 0.0f))
					* glm::scale(glm::mat4(1.0f), glm::vec3(_size.x * _scale, _size.y * _scale, 1.0f));

	_shader->bind();
	_shader->setMat4("model", model);
	_shader->setInt("tex", 0);
	_shader->setMat4("projection", glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f));

	_activeTexture->bind(0);

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
}
