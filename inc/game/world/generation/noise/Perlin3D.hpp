#pragma once

#include "game/world/generation/noise/White.hpp"
#include <algorithm>
#include <array>

namespace Noise
{
class Perlin3D
{
	public:
		Perlin3D() {}
		~Perlin3D() {}

		void	init(u32 seed)
		{
			_shuffle_permutations(seed);
		}

		double	sample(const vec3d& pos)
		{
			int _x = (int)floor(pos.x()) & 255;
			int _y = (int)floor(pos.y()) & 255;
			int _z = (int)floor(pos.z()) & 255;

			double xf = pos.x() - floor(pos.x());
			double yf = pos.y() - floor(pos.y());
			double zf = pos.z() - floor(pos.z());

			vec3f c000 = vec3f(xf, yf, zf);
			vec3f c100 = vec3f(xf - 1.0, yf, zf);
			vec3f c010 = vec3f(xf, yf - 1.0, zf);
			vec3f c110 = vec3f(xf - 1.0, yf - 1.0, zf);
			vec3f c001 = vec3f(xf, yf, zf - 1.0);
			vec3f c101 = vec3f(xf - 1.0, yf, zf - 1.0);
			vec3f c011 = vec3f(xf, yf - 1.0, zf - 1.0);
			vec3f c111 = vec3f(xf - 1.0, yf - 1.0, zf - 1.0);

			int v000 = _hash(_x, _y, _z);
			int v100 = _hash(_x + 1, _y, _z);
			int v010 = _hash(_x, _y + 1, _z);
			int v110 = _hash(_x + 1, _y + 1, _z);
			int v001 = _hash(_x, _y, _z + 1);
			int v101 = _hash(_x + 1, _y, _z + 1);
			int v011 = _hash(_x, _y + 1, _z + 1);
			int v111 = _hash(_x + 1, _y + 1, _z + 1);

			double d000 = vec3f::dot(c000, _constant_vector(v000));
			double d100 = vec3f::dot(c100, _constant_vector(v100));
			double d010 = vec3f::dot(c010, _constant_vector(v010));
			double d110 = vec3f::dot(c110, _constant_vector(v110));
			double d001 = vec3f::dot(c001, _constant_vector(v001));
			double d101 = vec3f::dot(c101, _constant_vector(v101));
			double d011 = vec3f::dot(c011, _constant_vector(v011));
			double d111 = vec3f::dot(c111, _constant_vector(v111));

			double u = _fade(xf);
			double v = _fade(yf);
			double w = _fade(zf);

			double x00 = lerp(d000, d100, u);
			double x10 = lerp(d010, d110, u);
			double x01 = lerp(d001, d101, u);
			double x11 = lerp(d011, d111, u);

			double y0 = lerp(x00, x10, v);
			double y1 = lerp(x01, x11, v);

			return (lerp(y0, y1, w));
		}

		double	sample_fbm(const vec3d& pos, float freq, int noisiness)
		{
			double	res = 0;
			double	amp = 0.5;
			for (int i = 0; i < noisiness; i++)
			{
				res += sample(vec3d(pos) * freq) * amp;
				freq *= 2.0;
				amp /= 2.0;
			}
			return (res);
		}
		double	sample_turbulence_fbm(const vec3d& pos, float freq, int noisiness)
		{
			double	res = 0;
			double	amp = 0.5;
			for (int i = 0; i < noisiness; i++)
			{
				res += std::abs(sample(vec3d(pos) * freq)) * amp;

				freq *= 2.0;
				amp /= 2.0;
			}

			return (res);
		}
		double	sample_ridge_fbm(const vec3d& pos, float freq, int noisiness)
		{
			double	res = 0;
			double	amp = 0.5;
			for (int i = 0; i < noisiness; i++)
			{
				res += sample(vec3d(pos) * freq) * amp;

				freq *= 2.0;
				amp /= 2.0;
			}

			return (1 - std::abs(res));
		}

	private:
		int	_hash(int x, int y, int z)
		{
			return (_permutations[_permutations[_permutations[x] + y] + z]);
		}

		vec3f	_constant_vector(int v)
		{
			int h = v & 15;

			switch (h)
			{
				case 0:
					return (vec3f(1,  1,  0));
				case 1:
					return (vec3f(-1,  1,  0));
				case 2:
					return (vec3f(1, -1,  0));
				case 3:
					return (vec3f(-1, -1,  0));
				case 4:
					return (vec3f(1,  0,  1));
				case 5:
					return (vec3f(-1,  0,  1));
				case 6:
					return (vec3f(1,  0, -1));
				case 7:
					return (vec3f(-1,  0, -1));
				case 8:
					return (vec3f(0,  1,  1));
				case 9:
					return (vec3f(0, -1,  1));
				case 10:
					return (vec3f(0,  1, -1));
				case 11:
					return (vec3f(0, -1, -1));
				case 12:
					return (vec3f(1,  1,  0));
				case 13:
					return (vec3f(-1,  1,  0));
				case 14:
					return (vec3f(0, -1,  1));
				default:
					return (vec3f(0, -1, -1));
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
