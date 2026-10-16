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
# include "colors.hpp"
# include "math/math.hpp"

enum BiomeT {
	Forest,
	Desert,
	Lava,
}; // TODO : move away

namespace Noise {
class Worley2D {
public:
	Worley2D() : _zoom(1.25), _seed(666777) {}
	~Worley2D() = default;

	// double sample(const vec2d& pos) const;
	BiomeT	sample(const vec2d& pos) const;

	void	printTerm(const vec2d& size);

private:

	vec2d _cellPoint(const vec2i32& cellPos) const;

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

	float	_zoom;
	u32		_seed;
};
};

#endif //MINECRAFT_WORLEY2D_HPP