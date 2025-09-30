/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Image.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:29:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 21:39:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_HPP
# define IMAGE_HPP

# include "UIElement.hpp"

class Image : public UIElement
{
	public:
		Image(Texture *texture, glm::vec2 offset, glm::vec2 anchor, glm::vec2 scale = glm::vec2(1))
			: _texture(texture)
		{
			_offset = offset;
			_anchor = anchor;
			_size = glm::vec2(texture->getWidth(), texture->getHeight()) * scale;
		}

		~Image()
		{
			glDeleteVertexArrays(1, &_VAO);
			glDeleteBuffers(1, &_VBO);
		}

		void	handleEvents(UIEvent) {}
		void	draw(Shader *shader, glm::vec2 windowSize)
		{
			_upload();

			float	scale = UIElement::getUiScale(windowSize);
			glm::vec2	scaledSize = _size * scale;

			float	x = (_anchor.x * windowSize.x) - (_anchor.x * scaledSize.x);
			float	y = (_anchor.y * windowSize.y) - (_anchor.y * scaledSize.y);

			glm::vec2	pos = glm::vec2(x, y) + (_offset * scale);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f))
							* glm::scale(glm::mat4(1.0f), glm::vec3(_size.x * scale, _size.y * scale, 1.0f));

			shader->bind();
			shader->setMat4("model", model);
			shader->setInt("tex", 0);

			_texture->bind(0);

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
		}
	private:
		void _upload(void)
		{
			if (_VAO != 0)
				return ;

			glGenVertexArrays(1, &_VAO);
			glGenBuffers(1, &_VBO);

			glBindVertexArray(_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			glBindVertexArray(0);
		}

		Texture		*_texture;
		uint		_VAO = 0;
		uint		_VBO;
		glm::vec2	_offset;
		glm::vec2	_anchor;
		glm::vec2	_size;
};

#endif
