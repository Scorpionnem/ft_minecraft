/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedImage.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 11:53:41 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 11:56:52 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "UIElement.hpp"

class	ImprovedImage : public UIElement
{
	public:
		ImprovedImage(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *shader, Texture *texture);
		~ImprovedImage() {}

		void	handleEvents(UIEvent) {}

		void	draw(glm::vec2 windowSize);

	private:
		Texture	*_texture = NULL;

		Shader	*_shader = NULL;
};
