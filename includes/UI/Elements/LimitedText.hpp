/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitedText.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 09:22:12 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 12:30:06 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMITEDTEXT_HPP
# define LIMITEDTEXT_HPP

# include "Text.hpp"

class	LimitedText : public Text
{
	public:
		LimitedText(const std::string &text, Texture *texture, glm::vec2 offset, glm::vec2 anchor, uint maxCharCount, glm::vec2 scale = glm::vec2(1))
			: Text(text, texture, offset, anchor, scale)
		{
			_maxCharCount = maxCharCount;
			_actualText = text;
		}
		void	draw(Shader *shader, glm::vec2 windowSize)
		{
			std::string	str;

			if (_actualText.size() > (_maxCharCount - 1))
				str += _actualText.substr(_actualText.size() - (_maxCharCount - 1), _actualText.size());
			else
				str = _actualText;
			this->_text = str;
			Text::draw(shader, windowSize);
		}
		void	setText(const std::string &text) {_actualText = text;}
		std::string	getText(void) {return (_actualText);}
	private:
		std::string		_actualText;
		uint			_maxCharCount;
};

#endif
