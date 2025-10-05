/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedBackgroundImage.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:51:15 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 12:10:36 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ImprovedBackgroundImage.hpp"

ImprovedBackgroundImage::ImprovedBackgroundImage(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, glm::vec2 scaleAxis, float darknessFactor, Shader *shader, Texture *texture)
{
	_texture = texture;
		
	_shader = shader;
		
	_size = size;
	_anchor = anchor;
	_offset = offset;
	_darknessFactor = darknessFactor;
	_scaleAxis = scaleAxis;
}

void	ImprovedBackgroundImage::draw(glm::vec2 windowSize)
{
	if (!_texture || !_shader)
		return ;

	_upload();

	glm::vec2	scale = UIElement::_getUiScaleXY(windowSize);
	scale = glm::vec2(_scaleAxis.x ? scale.x : UIElement::_getUiScale(windowSize), _scaleAxis.y ? scale.y : UIElement::_getUiScale(windowSize));
	glm::vec2 scaledSize = _size * scale;

	float	x = (_anchor.x * windowSize.x) - (_anchor.x * scaledSize.x);
	float	y = (_anchor.y * windowSize.y) - (_anchor.y * scaledSize.y);

	glm::vec2	pos = glm::vec2(x, y) + (_offset * scale);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f))
					* glm::scale(glm::mat4(1.0f), glm::vec3(scaledSize.x, scaledSize.y, 1.0f));

	_shader->bind();
	_shader->setMat4("model", model);
	_shader->setInt("tex", 0);
	_shader->setFloat("scale", UIElement::_getUiScale(windowSize));
	_shader->setFloat("darknessFactor", _darknessFactor);
	_shader->setMat4("projection", glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f));

	_texture->bind(0);

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
}
