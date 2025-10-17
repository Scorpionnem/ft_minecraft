/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedText.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 22:27:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/15 11:17:36 by mbatty           ###   ########.fr       */
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

extern std::map<char, int>	_charSpacing;

float	ImprovedText::computeStringSize()
{
	float	res = 0;
	for (char c : _text)
	{
		res += _charSpacing[c] + 1;
	}
	return (res);
}

void	ImprovedText::draw(glm::vec2 windowSize)
{
	if (!_texture || !_shader)
		return ;

	_upload();

	float uiScale = UIElement::_getUiScale(windowSize);

	_size = glm::vec2(DEFAULT_FONT_SIZE * _text.size(), DEFAULT_FONT_SIZE) * _scale;
	glm::vec2 spacedSize = glm::vec2(computeStringSize(), DEFAULT_FONT_SIZE);
	
	_pos = UIElement::_getScaledPos(spacedSize, _anchor, _offset, windowSize);

	glm::vec2 scaledSpacedSize = spacedSize * uiScale;

	glm::vec2	scaledSize = glm::vec2(_size.x * uiScale, _size.y * uiScale);

	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(_pos.x, _pos.y, 0.0f));
	model = glm::translate(model, glm::vec3(scaledSpacedSize.x / 2, scaledSpacedSize.y / 2, 0.0f));
	model = glm::rotate(model, glm::radians(_angle), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(-scaledSpacedSize.x / 2, -scaledSpacedSize.y / 2, 0.0f));
	model = glm::scale(model, glm::vec3(scaledSize.y, scaledSize.y, 1.0f));

	_shader->bind();
	_shader->setInt("tex", 0);
	_shader->setMat4("projection", glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f));
	_texture->bind(0);

	float	advance = 0;

	for (char c : _text)
	{
		float	spacing = _charSpacing[c];
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

		model = glm::translate(model, glm::vec3((spacing + 1) / 8.0f, 0.0f, 0.0f));
		advance += spacing + 1;
		if (_maxWidth > 0 && advance >= _maxWidth)
			break ;
	}
}
