/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedToggle.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 08:42:00 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/15 09:38:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPROVEDTOGGLE_HPP
# define IMPROVEDTOGGLE_HPP

# include "UIElement.hpp"

class	ImprovedToggle : public UIElement
{
	public:
		ImprovedToggle(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *shader, Texture *defaultTexture, Texture *hoverTexture, Texture *disabledTexture);
		~ImprovedToggle() {}

		void	handleEvents(UIEvent events);

		void	draw(glm::vec2 windowSize);

		void	setClickFunc(std::function<void(bool)> func) {_onClick = func;}
		void	disable() {_disabled = true;}
		void	enable() {_disabled = false;}
		void	unselect() {_clicked = false; if (_onClick) _onClick(_clicked);};
	private:
		Texture	*_activeTexture = NULL;

		Texture	*_defaultTexture = NULL;
		Texture	*_hoverTexture = NULL;
		Texture	*_disabledTexture = NULL;

		Shader	*_shader = NULL;

		bool	_disabled = false;
		bool	_clicked = false;

		std::function<void(bool)>	_onClick = NULL;
};

#endif
