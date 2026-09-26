#pragma once

#include "game/world/generation/noise/White.hpp"
#include <algorithm>

namespace Noise
{
	inline float easeIn(float interpolator)
	{
		return (interpolator * interpolator);
	}

	inline float easeOut(float interpolator)
	{
		return (1 - easeIn(1 - interpolator));
	}

	inline float easeInOut(float interpolator)
	{
		float	easeInValue = easeIn(interpolator);
		float	easeOutValue = easeOut(interpolator);
		return (lerp(easeInValue, easeOutValue, interpolator));
	}

	inline float perlin(vec2f value)
	{
		vec2f	lowerLeftDirection = rand2dTo2d(vec2f(floor(value.x()), floor(value.y()))) * 2 - 1;
		vec2f	lowerRightDirection = rand2dTo2d(vec2f(ceil(value.x()), floor(value.y()))) * 2 - 1;
		vec2f	upperLeftDirection = rand2dTo2d(vec2f(floor(value.x()), ceil(value.y()))) * 2 - 1;
		vec2f	upperRightDirection = rand2dTo2d(vec2f(ceil(value.x()), ceil(value.y()))) * 2 - 1;

		vec2f	fraction = frac(value);

		float	lowerLeftFunctionValue = vec2f::dot(lowerLeftDirection, fraction - vec2f(0, 0));
		float	lowerRightFunctionValue = vec2f::dot(lowerRightDirection, fraction - vec2f(1, 0));
		float	upperLeftFunctionValue = vec2f::dot(upperLeftDirection, fraction - vec2f(0, 1));
		float	upperRightFunctionValue = vec2f::dot(upperRightDirection, fraction - vec2f(1, 1));

		float	interpolatorX = easeInOut(fraction.x());
		float	interpolatorY = easeInOut(fraction.y());

		float	lowerCells = lerp(lowerLeftFunctionValue, lowerRightFunctionValue, interpolatorX);
		float	upperCells = lerp(upperLeftFunctionValue, upperRightFunctionValue, interpolatorX);

		float	noise = lerp(lowerCells, upperCells, interpolatorY);
		return (noise);
	}

	inline float	noise(const vec2f &pos, float freq, float amp, int noisiness)
	{
		float	res = 0;
		for (int i = 0; i < noisiness; i++)
		{
			res += perlin(vec2f(pos) * freq) * amp;

			freq *= 2;
			amp /= 2;
		}

		res = std::clamp(res, -1.0f, 1.0f);

		return (res);
	}
};
