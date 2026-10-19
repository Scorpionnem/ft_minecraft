//
// Created by bvasseur on 10/14/26.
//

#ifndef MINECRAFT_WORLEY2D_HPP
# define MINECRAFT_WORLEY2D_HPP

# include <bit>      // std::rotl (C++20)
# include <cstdint>
# include <array>
# include <functional>
# include <algorithm>
# include <memory>
# include "colors.hpp"
# include "math/math.hpp"
# include "../../../../../inc/game/world/generation/Biomes/ABiomeType.hpp"

namespace Noise {
class Worley2D {
public:
	Worley2D() : _seed(666777), _zoom(1.25), _sharpness(5), _k(2) {};
	~Worley2D() = default;

	// double sample(const vec2d& pos) const;
	// BiomeT	sample(const vec2d& pos) const;
	std::vector<std::pair<std::shared_ptr<ABiomeType>, double>>	sampleBiomes(const vec2d& pos) const;

	void	printTerm(const vec2d& size);

private:

	vec2d	_cellPoint(const vec2i32& cellPos) const;
	double	_valueNoise(const vec2d& p, double seed) const;
	double	_fbm(const vec2d& p, double seed) const;
	vec2d	_warp(const vec2d& p) const;

	double	rand1dTo1d(const double value, const double mutator = 0.546) const {
		return (frac(sin(value + mutator) * 148.5453));
	}

	const std::array<vec2i, 9> _neighborOffsets = {
		vec2i(-1, -1),
		vec2i(-1, -0),
		vec2i(-1,  1),
		vec2i( 0, -1),
		vec2i( 0,  0),
		vec2i( 0,  1),
		vec2i( 1, -1),
		vec2i( 1,  0),
		vec2i( 1,  1)
	};

	double _warpStrength = 0.;   // in cells; 0 = no warp (current behavior)
	double _warpFreq     = 0.5;  // how fast the warp varies, in cells
	int    _warpOctaves  = 4;

	u32		_seed;
	float	_zoom;
	float	_sharpness;
	short	_k;
};
};

#endif //MINECRAFT_WORLEY2D_HPP