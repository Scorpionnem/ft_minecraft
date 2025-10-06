/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Perlin2D.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:32:39 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/06 13:35:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERLIN2D_HPP
# define PERLIN2D_HPP

# include "libs.hpp"

namespace Perlin2D
{
	float	perlin2D(float x, float y);
	float	calcNoise(const glm::vec2 &pos, float freq, float amp, int noisiness);
};

#endif
