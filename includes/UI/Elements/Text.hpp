/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Text.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:23:37 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 10:21:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXT_HPP
# define TEXT_HPP

#include "UIElement.hpp"

# define DEFAULT_FONT_SIZE 8

/*
	Text ui element, self explanatory, just text on the screen
*/
class Text : public UIElement
{
	public:
		Text(const std::string &text, Texture *texture, Shader *shader, glm::vec2 offset, glm::vec2 anchor, glm::vec2 scale = glm::vec2(1))
			: _texture(texture)
		{
			_text = text;
			_offset = offset;
			_anchor = anchor;
			_size = glm::vec2(DEFAULT_FONT_SIZE) * scale;
			_scale = scale;
			_shader = shader;
		}

		~Text() {}

		virtual void	setText(const std::string &text) {_text = text;}
		void	setAngle(float angle) {_angle = angle;}
		virtual std::string	getText(void) {return (_text);}
		void	setScale(glm::vec2 scale) {_scale = scale;}
		void	setColor(glm::vec3 color) {_color = color;}

		void	handleEvents(UIEvent) {}
		virtual void	draw(glm::vec2 windowSize)
		{
			_upload();

			float	scale = UIElement::getUiScale(windowSize);
			_size = glm::vec2(DEFAULT_FONT_SIZE) * _scale;
			glm::vec2	scaledSize = glm::vec2((_size.x * _text.size()) * scale, _size.y * scale);

			float	x = (_anchor.x * windowSize.x) - (_anchor.x * scaledSize.x);
			float	y = (_anchor.y * windowSize.y) - (_anchor.y * scaledSize.y);

			glm::vec2	pos = glm::vec2(x, y) + (_offset * scale);

			glm::mat4 model(1.0f);

			model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
			model = glm::translate(model, glm::vec3(((_size.x * _text.size()) * scale) / 2, (_size.y * scale) / 2, 0.0f));
			model = glm::rotate(model, glm::radians(_angle), glm::vec3(0.0, 0.0, 1.0));
			model = glm::translate(model, glm::vec3(-((_size.x * _text.size()) * scale) / 2, -(_size.y * scale) / 2, 0.0f));
			model = glm::scale(model, glm::vec3(_size.x * scale, _size.y * scale, 1.0f));

			_shader->bind();
			_shader->setInt("tex", 0);
			_texture->bind(0);

			for (char c : _text)
			{
				_shader->setVec3("color", _color / 4.f);
				_shader->setMat4("model", glm::translate(model, glm::vec3(1.f / 8.f, 1.f / 8.f, 0.0f)));
				_shader->setInt("charIndex", c);

				glBindVertexArray(_VAO);
				glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));

				_shader->setVec3("color", _color);
				_shader->setMat4("model", model);
				_shader->setInt("charIndex", c);
				_shader->setMat4("projection", glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, -1.f, 1.f));

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
		glm::vec2	_scale;
		glm::vec3	_color = glm::vec3(1);
		float		_angle = 0;
};

#endif
