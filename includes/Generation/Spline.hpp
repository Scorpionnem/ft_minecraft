/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Spline.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:25:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/06 17:29:05 by mbatty           ###   ########.fr       */
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

float	getValueInSpline(const Spline &spline, float value);

extern const Spline CONTINENTALNESS_SPLINE;
extern const Spline RIVERS_VALLEYS_SPLINE;

#endif
