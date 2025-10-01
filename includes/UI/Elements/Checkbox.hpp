/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checkbox.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:29:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 14:57:05 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKBOX_HPP
# define CHECKBOX_HPP

# include "UIElement.hpp"

/*
	Checkbox ui element, when pressed it will be set as checked, if you click again on it it will be unselected

	onClick function takes a boolean that tells wether the checkbox is clicked or not when its executed
*/
class Checkbox : public UIElement
{
	public:
		/*
			Scale defaults to the default texture

			@param texture default texture
			@param texture hover texture
		*/
		Checkbox(Texture *texture, Texture *hoverTexture, glm::vec2 offset, glm::vec2 anchor, glm::vec2 scale = glm::vec2(1))
			: _currentTexture(texture), _texture(texture), _hoverTexture(hoverTexture)
		{
			_offset = offset;
			_anchor = anchor;
			_size = glm::vec2(texture->getWidth(), texture->getHeight()) * scale;
		}
		void	setClickFunc(std::function<void(bool)> func)
		{
			_onClick = func;
		}
		void	setChecked(bool state)
		{
			_checked = state;
		}

		~Checkbox() {}

		void	handleEvents(UIEvent events)
		{
			float		scale = UIElement::getUiScale(events.windowSize);
			glm::vec2	pos = UIElement::_getScaledPos(_size, _anchor, _offset, events.windowSize);

			bool	inside = _isInBounds(events.mousePos, pos, this->_size * scale);

			if (inside || _checked)
				_currentTexture = _hoverTexture;
			else
				_currentTexture = _texture;

			if (inside && events.inputs->isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
			{
				_checked = !_checked;
				if (_onClick)
					_onClick(_checked);
			}
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

		bool						_checked = false;
		std::function<void(bool)>	_onClick = NULL;
};

#endif
