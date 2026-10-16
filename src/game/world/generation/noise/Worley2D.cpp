//
// Created by bvasseur on 10/15/26.
//

#include "game/world/generation/noise/Worley2D.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <unordered_map>
#include <vector>


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

std::array<std::shared_ptr<ABiomeType>, 4> biomeList = {
	std::make_shared<Forest>(),
	std::make_shared<Desert>(),
	std::make_shared<Lava>(),
	std::make_shared<Ocean>(),
};

std::vector<std::pair<std::shared_ptr<ABiomeType>, double>> Noise::Worley2D::sample(const vec2d& pos) const {
	const vec2d newPos = pos / _zoom;
	const vec2i32 cellPos = floor(newPos);
	std::unordered_map<std::shared_ptr<ABiomeType>, double> biomes_map;
	std::vector<std::pair<std::shared_ptr<ABiomeType>, double>> biomes;
	double sumWeights = 0.;

	for (const vec2i32& neighborOffset : _neighborOffsets) {
		const vec2d cPos = _cellPoint(vec2i32(cellPos.x() + neighborOffset.x(), cellPos.y() + neighborOffset.y()));
		const double dist = vec2d::dot(newPos - cPos, newPos - cPos);
		const u64 randomValue = rand1dTo1d(cPos.y() * cPos.x(), _seed / 25.95301) * 1000;
		const auto closeBiome = biomeList[randomValue % biomeList.size()];
		if (biomes_map.find(closeBiome) == biomes_map.end() || biomes_map[closeBiome] > dist)
			biomes_map[closeBiome] = dist;
		// biomes.push_back({closeBiome, dist});
	}

	for (const auto& [b, dist] : biomes_map)
		biomes.push_back({b, dist});
	std::sort(biomes.begin(), biomes.end(), [](const auto& a, const auto& b) { return (a.second < b.second); });
	if (_k < (short)biomes.size())
		biomes.erase(biomes.begin() + _k, biomes.end());
	for (auto& [b, dist] : biomes) {
		// PRINT "Biome: " << b->getName() << ", dist: " << dist ENDL;
		double weight = (dist < 1e-9) ? 1. : 1. / std::pow(dist, _sharpness);
		dist = weight;
		sumWeights += weight;
	}
	// PRINT RED "BLEH" << RESET ENDL;

	(void)_k;
	for (auto& [b, percentage] : biomes)
		percentage /= sumWeights;
	return biomes;
}

void	Noise::Worley2D::printTerm(const vec2d& size) {
	_zoom = 2.25;
	_sharpness = 5.;
	_k = 4;
	std::string str;
	const std::function<vec3i(vec2d)> getColor = [&](const vec2d &pos) -> vec3i {
		auto biomes = sample(pos);
		vec3d total = 0;
		for (const auto& [b, percentage] : biomes) {
			total += b->getTermColor() * percentage;
			// PRINT "Biome: " << b->getName() << ", percentage: \t" << percentage  << RESET ENDL;
		}
		// PRINT BLU "Total: " << total << RESET ENDL;
		return (total);
	};

	(void)size;
	for (double y = 0; y < size.y(); y += 0.2) {
		for (double x = 0; x < size.x(); x += 0.1) {
			vec3i color = getColor({x, y});
			str += RGB_BG + RGB_CODE_VEC(color);
			color = getColor({x, y + 0.1});
			str += RGB_FG + RGB_CODE_VEC(color) + DBLOCK;
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
