/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Button.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:29:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 20:58:18 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUTTON_HPP
# define BUTTON_HPP

# include "UIElement.hpp"

class Button : public UIElement
{
	public:
		/*
			Scale defaults to the default texture

			@param texture default texture
			@param texture hover texture
		*/
		Button(Texture *texture, Texture *hoverTexture, glm::vec2 offset, glm::vec2 anchor, glm::vec2 scale = glm::vec2(1))
			: _currentTexture(texture), _texture(texture), _hoverTexture(hoverTexture)
		{
			_offset = offset;
			_anchor = anchor;
			_size = glm::vec2(texture->getWidth(), texture->getHeight()) * scale;
			_upload();
		}
		void	setClickFunc(std::function<void(void)> func)
		{
			_onClick = func;
		}

		~Button()
		{
			glDeleteVertexArrays(1, &_VAO);
			glDeleteBuffers(1, &_VBO);
		}

		void	handleEvents(UIEvent events)
		{
			float		scale = UIElement::getUiScale(events.windowSize);
			glm::vec2	pos = UIElement::_getScaledPos(_size, _anchor, _offset, events.windowSize);

			bool	inside = _isInBounds(events.mousePos, pos, this->_size * scale);

			if (inside)
				_currentTexture = _hoverTexture;
			else
				_currentTexture = _texture;

			if (inside && events.inputs->isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				if (!_prevClicked && _onClick)
					_onClick();
				_prevClicked = true;
			}
			else
				_prevClicked = false;
		}
		void	draw(Shader *shader, glm::vec2 windowSize)
		{
			float		scale = UIElement::getUiScale(windowSize);
			glm::vec2	pos = UIElement::_getScaledPos(_size, _anchor, _offset, windowSize);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f))
							* glm::scale(glm::mat4(1.0f), glm::vec3(_size.x * scale, _size.y * scale, 1.0f));

			shader->bind();
			shader->setMat4("model", model);
			shader->setInt("tex", 0);

			_currentTexture->bind(0);

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
		}
	private:
		void _upload(void)
		{
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

		Texture		*_currentTexture;
		Texture		*_texture;
		Texture		*_hoverTexture;
		uint		_VAO;
		uint		_VBO;
		glm::vec2	_offset;
		glm::vec2	_anchor;
		glm::vec2	_size;

		bool						_prevClicked = false;
		std::function<void(void)>	_onClick = NULL;
};

#endif
