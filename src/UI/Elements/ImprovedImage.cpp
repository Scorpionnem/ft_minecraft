/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedImage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:51:15 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 12:17:55 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ImprovedImage.hpp"

ImprovedImage::ImprovedImage(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *shader, Texture *texture)
{
	_texture = texture;
		
	_shader = shader;
		
	_size = size;
	_anchor = anchor;
	_offset = offset;
}

void	ImprovedImage::draw(glm::vec2 windowSize)
{
	if (!_texture || !_shader)
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

	_texture->bind(0);

	glBindVertexArray(_VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
}
