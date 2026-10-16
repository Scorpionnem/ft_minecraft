//
// Created by bvasseur on 10/15/26.
//

#include "game/world/generation/noise/Worley2D.hpp"

#include <array>


constexpr std::array<BiomeT, 3> biomeList = {
	Forest,
	Desert,
	Lava,
};

// double Noise::Worley2D::sample(const vec2d& pos) const {
// 	const vec2d newPos = pos / _zoom;
// 	const vec2i32 cellPos = floor(newPos);
// 	double closest = std::numeric_limits<double>::max();
//
// 	for (const vec2i32& neighborOffset : _neighborOffsets) {
// 		const vec2d cPos = _cellPoint(vec2i32(cellPos.x() + neighborOffset.x(), cellPos.y() + neighborOffset.y()));
// 		const double dist = vec2d::dot(newPos - cPos, newPos - cPos);
// 		if (dist < closest) closest = dist;
// 	}
// 	return closest;
// }

BiomeT Noise::Worley2D::sample(const vec2d& pos) const {
	const vec2d newPos = pos / _zoom;
	const vec2i32 cellPos = floor(newPos);
	double closest = std::numeric_limits<double>::max();
	BiomeT closestBiome = BiomeT::Forest;

	for (const vec2i32& neighborOffset : _neighborOffsets) {
		const vec2d cPos = _cellPoint(vec2i32(cellPos.x() + neighborOffset.x(), cellPos.y() + neighborOffset.y()));
		const double dist = vec2d::dot(newPos - cPos, newPos - cPos);
		if (dist < closest) {
			closest = dist;
			u64 randomValue = rand1dTo1d(cPos.y() * cPos.x(), _seed / 25.95301) * 1000;
			closestBiome = biomeList[randomValue % biomeList.size()];
		}
	}
	return closestBiome;
}

void	Noise::Worley2D::printTerm(const vec2d& size) {
	_zoom = 1.25;
	std::string str;
	const std::function<std::string(vec2d)> getColor = [&](const vec2d &pos) -> std::string {
		BiomeT biome = sample(pos);
		switch(biome) {
		case Forest: return ";50;148;52m"; break;
		case Desert: return ";222;177;31m"; break;
		case Lava: return ";242;64;19m"; break;
		}
	};

	(void)size;
	for (double y = 0; y < size.y(); y += 0.2) {
		for (double x = 0; x < size.x(); x += 0.1) {
			str += RGB_BG + getColor({x, y});
			str += RGB_FG + getColor({x, y + 0.1}) + DBLOCK;
		}
		str += RESET "\n";
	}
	// str = "\033[" + std::to_string(size.y()) + "A\033[H" + str;
	std::cout << str << RESET;
}


/*
void	Noise::Worley2D::printTerm(const vec2d& size) {
	_zoom = 5.25;
	std::string str;
	const std::function<std::string(vec2d)> getColor = [&](const vec2d &pos) -> std::string {
		// PRINT RED "for pos " << pos << RESET ENDL;
		// if ((frac(pos.x()) > -0.1 && 0.1 > frac(pos.x())) || (frac(pos.y()) > -0.1 && 0.1 > frac(pos.y())))
			// return "255";
		// if (vec2d::length(_cellPoint(pos) - pos) < 0.05)
			// return "255";
		auto val = sample(pos) / 1. * 255.;
		// if (val > 255.)
			// PRINT PRP "val: " << val / 255. << RESET ENDL;
		val = std::clamp(val, 0., 255.);
		return std::to_string(static_cast<int>(val));
	};

	(void)size;
	for (double y = 0; y < size.y(); y += 0.2) {
		for (double x = 0; x < size.x(); x += 0.1) {
			std::string color = getColor({x, y}); // ? The macro calls the function 3 times if put as param
			str += RGB_BG + RGB_CODE_STR(color);
			color = getColor({x, y + 0.1});
			str += RGB_FG + RGB_CODE_STR(color) + DBLOCK;
		}
		str += RESET "\n";
	}
	// str = "\033[" + std::to_string(size.y()) + "A\033[H" + str;
	std::cout << str << RESET;

	// for (int y = 0; y < size.y(); y += 1) {
	// 	for (int x = 0; x < size.x(); x += 1) {
	// 		_cellPoint({x, y});
	// 	}
	// }
}
*/

vec2d Noise::Worley2D::_cellPoint(const vec2i32& cellPos) const {
	double key = rand1dTo1d(cellPos.y() * cellPos.x(), _seed / 45.5145);
	vec2d test = vec2d(
		cellPos.x() + rand1dTo1d(key, 78.233 * _seed),
		cellPos.y() + rand1dTo1d(cellPos.x() / key + _seed, key * 11.135 * _seed));
	// PRINT RED "cellPos: " << cellPos << ", test: " << test << RESET ENDL;
	return (test);
}
