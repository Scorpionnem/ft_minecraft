#pragma once

#include "game/world/Chunk.hpp"
#include "game/world/Block.hpp"
#include "game/world/render/mesh/ChunkMeshData.hpp"
#include <array>
#include <memory>

class ChunkMesher
{
	public:
		static ChunkMeshData buildMesh(const Chunk& chunk,
									   const std::array<chunkPtr, 6>& neighbours,
									   BlockRegistry& blocks);
};
