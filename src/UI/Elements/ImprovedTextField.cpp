/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedTextField.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 09:03:32 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 13:26:21 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ImprovedTextField.hpp"

ImprovedTextField::ImprovedTextField(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *fontShader, Shader *shader, Texture *fontTexture, Texture *defaultTexture, Texture *hoverTexture)
: _text("", 1, anchor, offset + glm::vec2(4, size.y / 4), fontShader, fontTexture), _toggle(size, anchor, offset, shader, defaultTexture, hoverTexture, defaultTexture)
{
	_size = size;
	_text.setMaxWidth(size.x - 12);
	_text.setOffsetY(offset.y + (anchor.y * -(DEFAULT_FONT_SIZE / 2)) + ((1 - anchor.y) * (DEFAULT_FONT_SIZE / 2)));
	_toggle.setClickFunc([this](bool state)
	{
		_selected = state;
		_cursorLast = glfwGetTime();
		if (!state && _onClick)
			_onClick(_input);
	});
}

void	ImprovedTextField::handleEvents(UIEvent events)
{
	_toggle.handleEvents(events);

	if (_selected)
	{
		for (uint key : events.inputs->getCharInputs())
			_input += key;
	}
	if (_selected && _input.size() && (events.inputs->isKeyPressed(GLFW_KEY_BACKSPACE) || events.inputs->isKeyRepeating(GLFW_KEY_BACKSPACE)))
		_input = _input.substr(0, _input.size() - 1);
	if (_selected && (events.inputs->isKeyPressed(GLFW_KEY_ESCAPE) || events.inputs->isKeyPressed(GLFW_KEY_ENTER)))
		_toggle.unselect();
}

void	ImprovedTextField::draw(glm::vec2 windowSize)
{
	_handleCursorBlink();

	_toggle.draw(windowSize);
	_text.draw(windowSize);
}

void	ImprovedTextField::_handleCursorBlink()
{
	if (_selected)
	{
		if (glfwGetTime() - _cursorLast > CURSOR_BLINK_TIME)
		{
			_text.setText(_input + " ");
			if (glfwGetTime() - _cursorLast > CURSOR_BLINK_TIME * 2)
				_cursorLast = glfwGetTime();
		}
		else
			_text.setText(_input + "_");
	}
	else
		_text.setText(_input);
}
