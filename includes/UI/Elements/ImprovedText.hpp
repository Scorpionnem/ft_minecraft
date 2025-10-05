/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedText.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:55:27 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 16:36:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "UIElement.hpp"

# define DEFAULT_FONT_SIZE 8

class	ImprovedText : public UIElement
{
	public:
		ImprovedText(const std::string &text, float scale, const glm::vec2 &anchor, const glm::vec2 &offset, Shader *shader, Texture *texture);
		~ImprovedText() {}

		void	handleEvents(UIEvent) {}

		void	draw(glm::vec2 windowSize);

		void		setText(const std::string &text) {_text = text;}
		std::string	getText(void) {return (_text);}

		void		setColor(const glm::vec4 &color) {_color = color;}
		void		setAngle(float angle) {_angle = angle;}
		void		setScale(float scale) {_scale = scale;}
	private:
		Texture	*_texture = NULL;

		Shader	*_shader = NULL;

		float		_scale = 1;
		float		_angle = 0;

		glm::vec4	_color = glm::vec4(1);

		std::string	_text;
};
