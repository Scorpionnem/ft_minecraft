#include "game/world/generation/NoiseGenerator.hpp"
#include "game/world/Chunk.hpp"
#include "game/world/Block.hpp"
#include "game/world/utils/Positions.hpp"

void NoiseGenerator::generateChunk(Chunk& chunk, u32 seed)
{
	Noise::Perlin2D	noise;
	noise.init(seed);

	chunkLocalVec3i	blockPos;
	for (blockPos.x() = 0; blockPos.x() < Chunk::SIZE; blockPos.x()++)
	{
		for (blockPos.z() = 0; blockPos.z() < Chunk::SIZE; blockPos.z()++)
		{
			worldVec3i worldPos = chunkLocalToWorld(blockPos, chunk.pos(), Chunk::SIZE);
			int	y = -noise.sample_turbulence_fbm(vec2f(worldPos.x(), worldPos.z()), 0.00125, 6) * 64;

			for (blockPos.y() = 0; blockPos.y() < Chunk::SIZE; blockPos.y()++)
			{
				worldVec3i worldPos2 = chunkLocalToWorld(blockPos, chunk.pos(), Chunk::SIZE);

				float	n = Noise::rand2dTo1d(vec2i(worldPos.x(), worldPos.z()));
				if (worldPos2.y() == y + 1 && n < 0.01f)
					chunk.setBlock(blockPos, Block::BLUE_ORCHID);
				else if (worldPos2.y() == y + 1 && n < 0.1f)
					chunk.setBlock(blockPos, Block::GRASS);
				if (worldPos2.y() == y)
					chunk.setBlock(blockPos, Block::GRASS_BLOCK);
				else if (worldPos2.y() < y)
					chunk.setBlock(blockPos, Block::STONE);
			}
		}
	}
	chunk.setState(Chunk::State::GENERATED);
}
