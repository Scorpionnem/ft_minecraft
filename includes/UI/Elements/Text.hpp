/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Text.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:23:37 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 09:51:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXT_HPP
# define TEXT_HPP

#include "UIElement.hpp"

# define DEFAULT_FONT_SIZE 8

class Text : public UIElement
{
	public:
		Text(const std::string &text, Texture *texture, glm::vec2 offset, glm::vec2 anchor, glm::vec2 scale = glm::vec2(1))
			: _texture(texture)
		{
			_text = text;
			_offset = offset;
			_anchor = anchor;
			_size = glm::vec2(DEFAULT_FONT_SIZE) * scale;
		}

		~Text() {}

		virtual void	setText(const std::string &text) {_text = text;}
		virtual std::string	getText(void) {return (_text);}

		void	handleEvents(UIEvent) {}
		virtual void	draw(Shader *shader, glm::vec2 windowSize)
		{
			_upload();

			float	scale = UIElement::getUiScale(windowSize);
			glm::vec2	scaledSize = glm::vec2((_size.x * _text.size()) * scale, _size.y * scale);

			float	x = (_anchor.x * windowSize.x) - (_anchor.x * scaledSize.x);
			float	y = (_anchor.y * windowSize.y) - (_anchor.y * scaledSize.y);

			glm::vec2	pos = glm::vec2(x, y) + (_offset * scale);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(_size.x * scale, _size.y * scale, 1.0f));

			shader->bind();
			shader->setInt("tex", 0);
			_texture->bind(0);

			for (char c : _text)
			{
				shader->setMat4("model", model);
				shader->setInt("charIndex", c);
				glBindVertexArray(_VAO);
				glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
				model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}
	protected:

		Texture		*_texture;
		std::string	_text;
		glm::vec2	_offset;
		glm::vec2	_anchor;
		glm::vec2	_size;
};

#endif
