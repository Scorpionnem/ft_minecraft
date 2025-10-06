/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spline.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:26:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/06 17:28:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spline.hpp"

const Spline CONTINENTALNESS_SPLINE =
{
	{
		{ -1.0f, 20},  // deep ocean
		{ -0.3f, 62},  // shallow ocean
		{ -0.15f,  64},  // plains
		{ 0.1f,  72},  // plains
		{ 0.5f,  120},  // hills
		{ 1.0f,  255.0f}  // mountains
	}
};

const Spline RIVERS_VALLEYS_SPLINE =
{
	{
		{0, -15},
		{0.08, 0},
		{0.5, 0},
		{0.6, 32},
		{1, 64}
	}
};

float	getValueInSpline(const Spline &spline, float value)
{
	if (value <= spline.points.front().x)
		return (spline.points.front().y);
	if (value >= spline.points.back().x)
		return (spline.points.back().y);

	for (size_t i = 0; i < spline.points.size() - 1; i++)
	{
		const SplinePoint& p1 = spline.points[i];
		const SplinePoint& p2 = spline.points[i + 1];

		if (value >= p1.x && value <= p2.x)
		{
			float t = (value - p1.x) / (p2.x - p1.x);
			return (glm::mix(p1.y, p2.y, t));
		}
	}
	return (0.0f);
}
