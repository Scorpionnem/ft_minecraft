/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIElement.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:23:28 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 11:43:28 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UIElement.hpp"

bool	UIElement::_isInBounds(glm::vec2 point, glm::vec2 zonePos, glm::vec2 zoneSize)
{
	return (point.x >= zonePos.x && point.x <= zonePos.x + zoneSize.x && point.y >= zonePos.y && point.y <= zonePos.y + zoneSize.y);
}

glm::vec2	UIElement::_getScaledPos(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, glm::vec2 windowSize)
{
	float	scale = UIElement::_getUiScale(windowSize);
	glm::vec2	scaledSize = size * scale;

	float	x = (anchor.x * windowSize.x) - (anchor.x * scaledSize.x);
	float	y = (anchor.y * windowSize.y) - (anchor.y * scaledSize.y);

	return (glm::vec2(x, y) + (offset * scale));
}

float	UIElement::_getUiScale(glm::vec2 windowSize)
{
	float windowWidth  = windowSize.x;
	float windowHeight = windowSize.y;

	float scaleX = windowWidth / REFERENCE_WIDTH;
	float scaleY = windowHeight / REFERENCE_HEIGHT;

	return (std::min(scaleX, scaleY));
}

glm::vec2	UIElement::_getUiScaleXY(glm::vec2 windowSize)
{
	float windowWidth  = windowSize.x;
	float windowHeight = windowSize.y;

	float scaleX = windowWidth / REFERENCE_WIDTH;
	float scaleY = windowHeight / REFERENCE_HEIGHT;

	return (glm::vec2(scaleX, scaleY));
}

void	UIElement::_upload(void)
{
	if (_VAO != 0)
		return ;

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}
