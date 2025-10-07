/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spline.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:26:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/07 20:20:42 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Spline.hpp"

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
