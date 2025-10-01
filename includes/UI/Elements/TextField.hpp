/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextField.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:45:19 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 10:24:11 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTFIELD_HPP
# define TEXTFIELD_HPP

# include "LimitedText.hpp"
# include "Toggle.hpp"

class	TextField : public UIElement
{
	public:
		TextField(Texture *texture, Texture *hoverTexture, Texture *font, glm::vec2 offset, glm::vec2 anchor, uint maxCharCount, glm::vec2 scale = glm::vec2(1))
			: _text("", font, offset, anchor, maxCharCount, scale), _toggle(texture, hoverTexture, offset, anchor, scale)
		{
			_toggle.setClickFunc([this](bool state)
			{
				_selected = state;
				if (!state && _onClick)
					_onClick(this->_text.getText());
			});
		}
		void	setClickFunc(std::function<void(std::string)> func)
		{
			_onClick = func;
		}

		~TextField() {}

		void	handleEvents(UIEvent events)
		{
			_toggle.handleEvents(events);

			std::string	str = _text.getText();

			if (_selected)
			{
				for (uint key : events.inputs->getCharInputs())
					_text.setText(_text.getText() + (char)key);
			}
			if (events.inputs->isKeyPressed(GLFW_KEY_BACKSPACE) && str.size())
			{
				_text.setText(str.substr(0, str.size() - 1));
			}
		}
		void	draw(Shader *shader, glm::vec2 windowSize)
		{
			_toggle.draw(shader, windowSize);
			_text.draw(shader, windowSize);
			std::cout << "Dont use this draw call, real one takes 2 shaders (static_cast<TextField*>(this))" << std::endl;
		}
		void	draw(Shader *buttonShader, Shader *fontShader, glm::vec2 windowSize)
		{
			_toggle.draw(buttonShader, windowSize);
			_text.draw(fontShader, windowSize);
		}
	private:
		bool		_selected = false;
		LimitedText	_text;
		Toggle		_toggle;
		std::string	_defaultLabel;
		std::function<void(std::string)>	_onClick = NULL;
};

#endif
