/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextField.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:45:19 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 12:04:05 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTFIELD_HPP
# define TEXTFIELD_HPP

# include "LimitedText.hpp"
# include "Toggle.hpp"

# define CURSOR_BLINK_TIME 0.4

class	TextField : public UIElement
{
	public:
		TextField(Texture *texture, Texture *hoverTexture, Texture *font, glm::vec2 offset, glm::vec2 anchor, uint maxCharCount, glm::vec2 scale = glm::vec2(1))
			: _text("", font, offset, anchor, maxCharCount, scale), _toggle(texture, hoverTexture, offset, anchor, scale)
		{
			_toggle.setClickFunc([this](bool state)
			{
				_selected = state;
				_cursorLast = glfwGetTime();
				if (!state && _onClick)
					_onClick(_input);
			});
		}
		void	setClickFunc(std::function<void(const std::string &)> func)
		{
			_onClick = func;
		}

		~TextField() {}

		void	handleEvents(UIEvent events)
		{
			_toggle.handleEvents(events);

			if (_selected)
			{
				for (uint key : events.inputs->getCharInputs())
					_input += key;
			}
			if (_input.size() && (events.inputs->isKeyPressed(GLFW_KEY_BACKSPACE) || events.inputs->isKeyRepeating(GLFW_KEY_BACKSPACE)))
				_input = _input.substr(0, _input.size() - 1);
			if (_selected && (events.inputs->isKeyPressed(GLFW_KEY_ESCAPE) || events.inputs->isKeyPressed(GLFW_KEY_ENTER)))
				_toggle.setChecked(false);
		}
		void	draw(Shader *buttonShader, Shader *fontShader, glm::vec2 windowSize)
		{
			_handleCursorBlink();

			_toggle.draw(buttonShader, windowSize);
			_text.draw(fontShader, windowSize);
		}

		void	draw(Shader *shader, glm::vec2 windowSize)
		{
			_toggle.draw(shader, windowSize);
			_text.draw(shader, windowSize);
			std::cout << "WARNING: Wrong draw call called line " << __LINE__ << " in file " << __FILE__ << std::endl;
		}
	private:
		void	_handleCursorBlink()
		{
			if (_selected)
			{
				if (glfwGetTime() - _cursorLast > CURSOR_BLINK_TIME)
				{
					_text.setText(_input + " ");
					if (glfwGetTime() - _cursorLast > CURSOR_BLINK_TIME * 2)
						_cursorLast = glfwGetTime();
				}
				else
					_text.setText(_input + "_");
			}
			else
				_text.setText(_input);
		}

		double		_cursorLast;
		std::string	_input;
		
		bool		_selected = false;
		LimitedText	_text;
		Toggle		_toggle;
		std::string	_defaultLabel;
		std::function<void(const std::string &)>	_onClick = NULL;
};

#endif
