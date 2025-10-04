/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedText.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 22:27:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/04 22:28:36 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ImprovedText.hpp"

ImprovedText::ImprovedText(const std::string &text, float scale, const glm::vec2 &anchor, const glm::vec2 &offset, Shader *shader, Texture *texture)
{
	_texture = texture;

	_shader = shader;

	_text = text;
	_scale = scale;
	_anchor = anchor;
	_offset = offset;
}

void	ImprovedText::draw(glm::vec2 windowSize)
{
	if (!_texture || !_shader)
		return ;

	_upload();

	float uiScale = UIElement::_getUiScale(windowSize);

	_size = glm::vec2(DEFAULT_FONT_SIZE) * _scale;
	glm::vec2	scaledSize = glm::vec2((_size.x) * uiScale, _size.y * uiScale);

	float	x = (_anchor.x * windowSize.x) - (_anchor.x * scaledSize.x);
	float	y = (_anchor.y * windowSize.y) - (_anchor.y * scaledSize.y);

	glm::vec2	pos = glm::vec2(x, y) + (_offset * uiScale);

	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
	model = glm::translate(model, glm::vec3(scaledSize.x / 2, scaledSize.y / 2, 0.0f));
	model = glm::rotate(model, glm::radians(_angle), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(-scaledSize.x / 2, -scaledSize.y / 2, 0.0f));
	model = glm::scale(model, glm::vec3(scaledSize.x, scaledSize.y, 1.0f));

	_shader->bind();
	_shader->setInt("tex", 0);
	_shader->setMat4("projection", glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f));
	_texture->bind(0);

	for (char c : _text)
	{
		_shader->setVec3("color", _color / 5.f);
		_shader->setMat4("model", glm::translate(model, glm::vec3(1.f / 8.f, 1.f / 8.f, 0.0f)));
		_shader->setInt("charIndex", c);

		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));

		_shader->setVec3("color", _color);
		_shader->setMat4("model", model);
		_shader->setInt("charIndex", c);

		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	}
}
