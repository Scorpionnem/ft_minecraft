/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quad.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:09:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/06 13:20:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "UIElement.hpp"

class	Quad : public UIElement
{
	public:
		Quad(glm::vec4 color, glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *shader)
		{
			_color = color;
			_size = size;
			_anchor = anchor;
			_offset = offset;
			_shader = shader;
		}
		~Quad() {}

		void	handleEvents(UIEvent) {}

		void	draw(glm::vec2 windowSize)
		{
			if (!_shader)
				return ;

			_upload();

			_scale = UIElement::_getUiScale(windowSize);
			_pos = UIElement::_getScaledPos(_size, _anchor, _offset, windowSize);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(_pos, 0.0f))
							* glm::scale(glm::mat4(1.0f), glm::vec3(_size.x * _scale, _size.y * _scale, 1.0f));

			_shader->bind();
			_shader->setMat4("model", model);
			_shader->setVec4("color", _color);
			_shader->setMat4("projection", glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f));

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
		}

	private:
		Shader	*_shader = NULL;

		glm::vec4	_color;
};
