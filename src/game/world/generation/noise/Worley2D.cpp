//
// Created by bvasseur on 10/15/26.
//

#include "game/world/generation/noise/Worley2D.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <unordered_map>
#include <vector>

std::array<std::shared_ptr<ABiomeType>, 4> biomeList = {
	std::make_shared<Forest>(),
	std::make_shared<Desert>(),
	std::make_shared<Lava>(),
	std::make_shared<Ocean>(),
};


/* ==================== METHODS ==================== */


std::vector<std::pair<std::shared_ptr<ABiomeType>, double>> Noise::Worley2D::sampleBiomes(const vec2d& pos) const {
	const vec2d newPos = _warp(pos / _zoom);
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
		double weight = (dist < 1e-9) ? 1. : 1. / std::pow(dist, _sharpness);
		dist = weight;
		sumWeights += weight;
	}

	for (auto& [b, percentage] : biomes)
		percentage /= sumWeights;
	return biomes;
}

void	Noise::Worley2D::printTerm(const vec2d& size) {
	_zoom = 7.25;
	_sharpness = 5.;
	_k = 4;
	_warpStrength = .9;
	_warpFreq = .8;
	_warpOctaves = 4;
	std::string str;
	const std::function<vec3i(vec2d)> getColor = [&](const vec2d &pos) -> vec3i {
		auto biomes = sampleBiomes(pos);
		vec3d total = 0;
		for (const auto& [b, percentage] : biomes) {
				total += b->getTermColor() * percentage;
		}
		return (total);
	};

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


/* ==================== PRIVATE METHODS ==================== */


// smooth 2D value noise in [0,1], built on rand1dTo1d
double Noise::Worley2D::_valueNoise(const vec2d& p, double seed) const {
	const double ix = std::floor(p.x());
	const double iy = std::floor(p.y());
	double fx = p.x() - ix;
	double fy = p.y() - iy;

	// smoothstep so the interpolation has no visible grid creases
	fx = fx * fx * (3. - 2. * fx);
	fy = fy * fy * (3. - 2. * fy);

	auto corner = [&](double cx, double cy) {
		return rand1dTo1d(cx * 127.1 + cy * 311.7, seed);
	};

	const double a = corner(ix,      iy);
	const double b = corner(ix + 1., iy);
	const double c = corner(ix,      iy + 1.);
	const double d = corner(ix + 1., iy + 1.);

	const double top    = a + (b - a) * fx;
	const double bottom = c + (d - c) * fx;
	return top + (bottom - top) * fy;
}

// fractal Brownian motion: several octaves of value noise, normalized to [0,1]
double Noise::Worley2D::_fbm(const vec2d& p, double seed) const {
	double sum = 0., amp = 0.5, norm = 0., freq = 1.;
	for (int i = 0; i < _warpOctaves; ++i) {
		sum  += amp * _valueNoise(vec2d(p.x() * freq, p.y() * freq), seed + i * 17.31);
		norm += amp;
		amp  *= 0.5;   // gain
		freq *= 2.;    // lacunarity
	}
	return sum / norm;
}

vec2d Noise::Worley2D::_warp(const vec2d& p) const {
	if (_warpStrength == 0.) return p;

	const vec2d q(p.x() * _warpFreq, p.y() * _warpFreq);
	// two independent fbm fields, one per axis
	const double wx = _fbm(q, _seed * 0.37 + 11.);
	const double wy = _fbm(q, _seed * 0.73 + 53.);

	// remap [0,1] -> [-1,1] and scale
	return vec2d(p.x() + (wx * 2. - 1.) * _warpStrength,
				 p.y() + (wy * 2. - 1.) * _warpStrength);
}

vec2d Noise::Worley2D::_cellPoint(const vec2i32& cellPos) const {
	double key = rand1dTo1d(cellPos.y() * cellPos.x(), _seed / 45.5145);
	vec2d test = vec2d(
		cellPos.x() + rand1dTo1d(key, 78.233 * _seed),
		cellPos.y() + rand1dTo1d(cellPos.x() / key + _seed, key * 11.135 * _seed));
	return (test);
}
