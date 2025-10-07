/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spline.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:25:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/07 20:35:57 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLINE_HPP
# define SPLINE_HPP

# include "libs.hpp"

struct	SplinePoint
{
	float	x;
	float	y;
};

struct	Spline
{
	std::vector<SplinePoint>	points;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SplinePoint, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Spline, points)

float	getValueInSpline(const Spline &spline, float value);

#endif
