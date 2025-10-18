/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedTextField.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 09:00:17 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 13:19:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPROVEDTEXTFIELD_HPP
# define IMPROVEDTEXTFIELD_HPP

# include "UIElement.hpp"
# include "ImprovedToggle.hpp"
# include "ImprovedText.hpp"

# define CURSOR_BLINK_TIME 0.4

class	ImprovedTextField : public UIElement
{
	public:
		ImprovedTextField(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *fontShader, Shader *shader, Texture *fontTexture, Texture *defaultTexture, Texture *hoverTexture);
		~ImprovedTextField() {}

		void	handleEvents(UIEvent events);
		void	draw(glm::vec2 windowSize);
		void	setClickFunc(std::function<void(const std::string &)> onClick) {_onClick = onClick;}
	private:
		void	_handleCursorBlink();
		
		ImprovedText	_text;
		ImprovedToggle	_toggle;

		std::string		_input;
		double			_cursorLast = 0;
		bool			_selected = false;
		
		std::function<void(const std::string &)>	_onClick = NULL;
};

#endif
