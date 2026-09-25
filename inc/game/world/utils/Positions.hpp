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

inline u64	hash(const vec3i& v)
{
	const u64 BITS = 21;
	const u64 MASK = (1ULL << BITS) - 1;
	const i64  BIAS = 1LL << (BITS - 1);

	u64 ux = static_cast<u64>(static_cast<i64>(v.x()) + BIAS) & MASK;
	u64 uy = static_cast<u64>(static_cast<i64>(v.y()) + BIAS) & MASK;
	u64 uz = static_cast<u64>(static_cast<i64>(v.z()) + BIAS) & MASK;

	return (ux << (BITS * 2)) | (uy << (BITS * 1)) | (uz << (BITS * 0));
}
