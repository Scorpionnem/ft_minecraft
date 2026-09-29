#pragma once

#include "math/math.hpp"
#include <vector>

struct ChunkVertex
{
	vec3f	pos;
	vec3f	normal;
	vec3f	color;
	vec2f	uv;
};

struct ChunkMeshData
{
	std::vector<ChunkVertex>	vertices;

	bool	empty() const { return vertices.empty(); }
	void	clear() { vertices.clear(); }
};
