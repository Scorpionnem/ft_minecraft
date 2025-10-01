/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Button.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:29:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 14:57:35 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUTTON_HPP
# define BUTTON_HPP

# include "UIElement.hpp"

/*
	Button ui element, when pressed it will call onClick

	onClick function takes void
*/
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
		}
		void	setClickFunc(std::function<void(void)> func)
		{
			_onClick = func;
		}

		~Button() {}

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
				if (_onClick)
					_onClick();
		}
		void	draw(Shader *shader, glm::vec2 windowSize)
		{
			_upload();

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

		Texture		*_currentTexture;
		Texture		*_texture;
		Texture		*_hoverTexture;
		glm::vec2	_offset;
		glm::vec2	_anchor;
		glm::vec2	_size;

		std::function<void(void)>	_onClick = NULL;
};

#endif
