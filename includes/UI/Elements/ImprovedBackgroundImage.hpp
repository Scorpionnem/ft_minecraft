/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedBackgroundImage.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:41:43 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 12:10:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "UIElement.hpp"

class	ImprovedBackgroundImage : public UIElement
{
	public:
		ImprovedBackgroundImage(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, glm::vec2 _scaleAxis, float darknessFactor, Shader *shader, Texture *texture);
		~ImprovedBackgroundImage() {}

		void	handleEvents(UIEvent) {}

		void	draw(glm::vec2 windowSize);

	private:
		Texture	*_texture = NULL;

		Shader	*_shader = NULL;
		
		float		_darknessFactor = 1;
		glm::vec2	_scaleAxis;
};
