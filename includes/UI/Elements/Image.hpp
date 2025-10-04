/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Image.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:29:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/04 21:27:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_HPP
# define IMAGE_HPP

# include "UIElement.hpp"

/*
	Text ui element, self explanatory, just a texture on the screen
*/
class Image : public UIElement
{
	public:
		Image(Texture *texture, Shader *shader, glm::vec2 offset, glm::vec2 anchor, glm::vec2 scale = glm::vec2(1))
			: _texture(texture)
		{
			_offset = offset;
			_anchor = anchor;
			_size = glm::vec2(texture->getWidth(), texture->getHeight()) * scale;
			_shader = shader;
		}

		~Image() {}

		void	handleEvents(UIEvent) {}
		void	draw(glm::vec2 windowSize)
		{
			if (!_texture || !_shader)
				return ;

			_upload();

			float	scale = UIElement::_getUiScale(windowSize);
			glm::vec2	scaledSize = _size * scale;

			float	x = (_anchor.x * windowSize.x) - (_anchor.x * scaledSize.x);
			float	y = (_anchor.y * windowSize.y) - (_anchor.y * scaledSize.y);

			glm::vec2	pos = glm::vec2(x, y) + (_offset * scale);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f))
							* glm::scale(glm::mat4(1.0f), glm::vec3(_size.x * scale, _size.y * scale, 1.0f));

			_shader->bind();
			_shader->setMat4("model", model);
			_shader->setInt("tex", 0);
			_shader->setMat4("projection", glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f));

			_texture->bind(0);

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
		}
	private:

		Texture		*_texture;
		glm::vec2	_offset;
		glm::vec2	_anchor;
		glm::vec2	_size;
};

#endif
