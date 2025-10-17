/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedButton.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:52:40 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/17 13:53:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//NEW VERSION OF THE BUTTON, NEED TO REPLACE ALL

#pragma once

# include "UIElement.hpp"

class	ImprovedButton : public UIElement
{
	public:
		ImprovedButton(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *shader, Texture *defaultTexture, Texture *hoverTexture, Texture *disabledTexture);
		~ImprovedButton() {}

		void	handleEvents(UIEvent events);

		void	draw(glm::vec2 windowSize);

		void	setClickFunc(std::function<void(void)> func) {_onClick = func;}
		void	disable() {_disabled = true;}
		void	enable() {_disabled = false;}
		bool	isHovered() {return (_hovered);}
		void	setHovered(bool state) {_hovered = state;}
	private:
		Texture	*_activeTexture = NULL;

		Texture	*_defaultTexture = NULL;
		Texture	*_hoverTexture = NULL;
		Texture	*_disabledTexture = NULL;

		Shader	*_shader = NULL;

		bool	_hovered = false;
		bool	_disabled = false;

		std::function<void(void)>	_onClick = NULL;
};
