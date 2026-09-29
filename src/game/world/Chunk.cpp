#include "game/world/Chunk.hpp"
#include "game/world/generation/NoiseGenerator.hpp"
#include "game/world/render/mesh/ChunkMesher.hpp"

void	Chunk::generate(u32 seed)
{
	NoiseGenerator	gen;
	gen.generateChunk(*this, seed);
}

void	Chunk::mesh(BlockRegistry& blocks, std::array<chunkPtr, 6> neighbours)
{
	setMeshData(ChunkMesher::buildMesh(*this, neighbours, blocks));
}
