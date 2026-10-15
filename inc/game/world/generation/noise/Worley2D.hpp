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

namespace Noise {
class Worley2D {
public:
	Worley2D() : _zoom(1.25), _seed(666777) {}
	~Worley2D() = default;

	double sample(const vec2d& pos) const;

	void	printTerm(const vec2d& size);

private:

	vec2d _cellPoint(const vec2i32& cellPos) const;

	double	rand1dTo1d(const double value, const double mutator = 0.546) const {
		return (frac(sin(value + mutator) * 148.5453));
	}



	static constexpr u64 FX_SEED = 0x517cc1b20a95ULL;

	static constexpr u64 fxStep(const u64 hash, const u64 word) {
		return (std::rotl(hash, 2) ^ word) * FX_SEED;   // u64 multiply wraps
	}

	static constexpr u64 _hashXY(const u64 seed, const vec2i32& p) {
		u64 h = 0;
		h = fxStep(h, seed);
		h = fxStep(h, static_cast<u64>(static_cast<u32>(p.x())));  // via u32: zero-extend
		h = fxStep(h, static_cast<u64>(static_cast<u32>(p.y())));
		return h;
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