#pragma once

#include "game/world/generation/noise/White.hpp"
#include <algorithm>
#include <array>

namespace Noise
{
class	Perlin2D
{
	public:
		Perlin2D() {}
		~Perlin2D() {}

		void	init(u32 seed)
		{
			_shuffle_permutations(seed);
		}

		double	sample(const vec2d& pos)
		{
			int _x = (int)floor(pos.x()) & 255;
			int _y = (int)floor(pos.y()) & 255;

			double xf = pos.x() - floor(pos.x());
			double yf = pos.y() - floor(pos.y());

			vec2f topRight = vec2f(xf - 1.0, yf - 1.0);
			vec2f topLeft = vec2f(xf, yf - 1.0);
			vec2f bottomRight = vec2f(xf - 1.0, yf);
			vec2f bottomLeft = vec2f(xf, yf);

			int	valueTopRight = _permutations[_permutations[_x + 1] + _y + 1];
			int	valueTopLeft = _permutations[_permutations[_x] + _y + 1];
			int	valueBottomRight = _permutations[_permutations[_x + 1] + _y];
			int	valueBottomLeft = _permutations[_permutations[_x] + _y];

			double dotTopRight    = vec2f::dot(topRight, _constant_vector(valueTopRight));
			double dotTopLeft     = vec2f::dot(topLeft, _constant_vector(valueTopLeft));
			double dotBottomRight = vec2f::dot(bottomRight, _constant_vector(valueBottomRight));
			double dotBottomLeft  = vec2f::dot(bottomLeft, _constant_vector(valueBottomLeft));

			double u = _fade(xf);
			double v = _fade(yf);

			return (lerp(lerp(dotBottomLeft, dotTopLeft, v), lerp(dotBottomRight, dotTopRight, v), u));
		}
		double	sample_fbm(const vec2d& pos, float freq, int noisiness)
		{
			double	res = 0;
			double	amp = 0.5;
			for (int i = 0; i < noisiness; i++)
			{
				res += sample(vec2f(pos) * freq) * amp;

				freq *= 2.0;
				amp /= 2.0;
			}

			return (res);
		}
		double	sample_turbulence_fbm(const vec2d& pos, float freq, int noisiness)
		{
			double	res = 0;
			double	amp = 0.5;
			for (int i = 0; i < noisiness; i++)
			{
				res += std::abs(sample(vec2f(pos) * freq)) * amp;

				freq *= 2.0;
				amp /= 2.0;
			}

			return (res);
		}
		double	sample_ridge_fbm(const vec2d& pos, float freq, int noisiness)
		{
			double	res = 0;
			double	amp = 0.5;
			for (int i = 0; i < noisiness; i++)
			{
				res += sample(vec2f(pos) * freq) * amp;

				freq *= 2.0;
				amp /= 2.0;
			}

			return (1 - std::abs(res));
		}
	private:
		vec2f _constant_vector(int v)
		{
			int h = v & 7;

			switch (h)
			{
				case 0:
					return (vec2f( 1.0,  0.0));
				case 1:
					return (vec2f(-1.0,  0.0));
				case 2:
					return (vec2f( 0.0,  1.0));
				case 3:
					return (vec2f( 0.0, -1.0));
				case 4:
					return (vec2f( 1.0,  1.0));
				case 5:
					return (vec2f(-1.0,  1.0));
				case 6:
					return (vec2f(-1.0, -1.0));
				default:
					return (vec2f( 1.0, -1.0));
			}
		}
		double	_fade(double t)
		{
			return ((6 * t - 15) * t + 10) * t * t * t;
		}
		void	_shuffle_permutations(u32 seed)
		{
			std::array<int, 256>	base;

			for (int i = 0; i < 256; i++)
				base[i] = i;

			for (size_t i = 0; i < base.size(); i++)
			{
				const size_t	index = static_cast<size_t>(Noise::rand1dTo1d(seed + i) * (float)i);

				std::swap(base[i], base[index]);
			}

			for (int i = 0; i < 256; i++)
			{
				_permutations[i] = base[i];
				_permutations[i + 256] = base[i];
			}
		}
		std::array<int, 512>	_permutations;
};
};
