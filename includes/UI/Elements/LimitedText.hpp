/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitedText.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:22:12 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 11:15:08 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMITEDTEXT_HPP
# define LIMITEDTEXT_HPP

# include "Text.hpp"

/*
	LimitedText ui element, extends the Text ui element by having a maxCharCount, it means that if it goes over maxCharCount,
	the string will only render the end of the string and not grow any longer
*/
class	LimitedText : public Text
{
	public:
		LimitedText(const std::string &text, Texture *texture, Shader *shader, glm::vec2 offset, glm::vec2 anchor, uint maxCharCount, glm::vec2 scale = glm::vec2(1))
			: Text(text, texture, shader, offset, anchor, scale)
		{
			_maxCharCount = maxCharCount;
			_actualText = text;
		}
		void	draw(glm::vec2 windowSize)
		{
			std::string	str;

			if (_actualText.size() > (_maxCharCount - 1))
				str += _actualText.substr(_actualText.size() - (_maxCharCount - 1), _actualText.size());
			else
				str = _actualText;
			this->_text = str;
			Text::draw(windowSize);
		}
		void	setText(const std::string &text) {_actualText = text;}
		std::string	getText(void) {return (_actualText);}
	private:
		std::string		_actualText;
		uint			_maxCharCount;
};

#endif
