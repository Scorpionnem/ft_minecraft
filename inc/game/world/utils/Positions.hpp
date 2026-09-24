#pragma once

#include "math/math.hpp"

using chunkWorldVec3i = vec3i;
using chunkLocalVec3i = vec3i;
using worldVec3i = vec3i;

inline constexpr int	floorDiv(int x, int d)
{
	return ((x >= 0) ? (x / d) : ((x - d + 1) / d));
}

inline constexpr int	floorMod(int x, int d)
{
	int m = x % d;
	return ((m < 0) ? (m + d) : m);
}

inline chunkWorldVec3i	worldToChunkWorld(const worldVec3i &pos, int chunk_size)
{
	return chunkWorldVec3i(
		floorDiv(pos.x(), chunk_size),
		floorDiv(pos.y(), chunk_size),
		floorDiv(pos.z(), chunk_size)
	);
}

inline worldVec3i	chunkLocalToWorld(const chunkLocalVec3i &pos, const chunkWorldVec3i &chunk_pos, int chunk_size)
{
	return (pos + (chunk_pos * chunk_size));
}

inline chunkLocalVec3i	worldToChunkLocal(const worldVec3i &pos, int chunk_size)
{
	return chunkLocalVec3i(
		floorMod(pos.x(), chunk_size),
		floorMod(pos.y(), chunk_size),
		floorMod(pos.z(), chunk_size)
	);
}
