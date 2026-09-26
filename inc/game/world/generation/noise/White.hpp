#pragma once

#include "math/math.hpp"

namespace Noise
{
	inline float rand3dTo1d(vec3f value, vec3f dotDir = vec3f(12.9898, 78.233, 37.719))
	{
		vec3f	smallValue = sin(value);
		float	random = vec3f::dot(smallValue, dotDir);
		random = frac(sin(random) * 143758.5453);
		return (random);
	}

	inline vec3f rand3dTo3d(vec3f value)
	{
		return vec3f(
			rand3dTo1d(value, vec3f(12.989, 78.233, 37.719)),
			rand3dTo1d(value, vec3f(39.346, 11.135, 83.155)),
			rand3dTo1d(value, vec3f(73.156, 52.235, 09.151))
		);
	}

	inline float	rand2dTo1d(vec2f value, vec2f dotDir = vec2f(12.9898, 78.233))
	{
		vec2f smallValue = sin(value);
		float random = vec3f::dot(smallValue, dotDir);
		random = frac(sin(random) * 143758.5453);
		return (random);
	}

	inline vec2f	rand2dTo2d(vec2f value)
	{
		return (vec2f(
			rand2dTo1d(value, vec2f(12.989, 78.233)),
			rand2dTo1d(value, vec2f(39.346, 11.135))
		));
	}

	inline float	rand1dTo1d(float value, float mutator = 0.546)
	{
		return (frac(sin(value + mutator) * 143758.5453));
	}
};
