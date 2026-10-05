#include "game/world/generation/OverworldGenerator.hpp"
#include "game/world/Chunk.hpp"
#include "game/world/Block.hpp"
#include "game/world/utils/Positions.hpp"

/*
	continentalness [-1, 1]

	[-1, -0.8] deep oceans
	[-0.8, -0.4] oceans
	[-0.4, 0.0] shallow oceans

	[0.0, 0.05] beachy
	[0.05, 0.20] flatty
	[0.20, 0.35] hilly
	[0.35, 0.5] plateau
	[0.5, 0.75] small mountains
	[0.75, 1] super high mountains
 */

using noiseRange = vec2f;

struct Biome
{
	noiseRange	continental_range;

	float	scale;
	int		noisiness;
	int		height;
	int		min_height;

	blockStateId	surface_block = 0;
};

void OverworldGenerator::generateChunk(Chunk& chunk, u32 seed)
{
	const std::array<Biome, 9>	biomes =
	{
		Biome{.continental_range = noiseRange(-1, 0.8), .scale = 0.001, .noisiness = 4, .height = 16, .min_height = -64, .surface_block = Block::SNOW}, // deep ocean
		Biome{.continental_range = noiseRange(-0.8, -0.15), .scale = 0.005, .noisiness = 3, .height = 32, .min_height = -32, .surface_block = Block::COBBLESTONE}, // ocean
		Biome{.continental_range = noiseRange(-0.15, 0.0), .scale = 0.015, .noisiness = 2, .height = 16, .min_height = -8, .surface_block = Block::STONE}, // shallow ocean
		Biome{.continental_range = noiseRange(0.0, 0.05), .scale = 0.02, .noisiness = 3, .height = 8, .min_height = 4, .surface_block = Block::SAND}, // beach
		Biome{.continental_range = noiseRange(0.05, 0.2), .scale = 0.003, .noisiness = 1, .height = 64, .min_height = 8, .surface_block = Block::GRASS_BLOCK}, // flat
		Biome{.continental_range = noiseRange(0.2, 0.35), .scale = 0.0014, .noisiness = 4, .height = 64, .min_height = 24, .surface_block = Block::MOSS}, // hill
		Biome{.continental_range = noiseRange(0.35, 0.5), .scale = 0.01, .noisiness = 3, .height = 16, .min_height = 48, .surface_block = Block::COBBLESTONE}, // plateau
		Biome{.continental_range = noiseRange(0.5, 0.75), .scale = 0.01, .noisiness = 1, .height = 64, .min_height = 64, .surface_block = Block::STONE}, // small mountain
		Biome{.continental_range = noiseRange(0.75, 1), .scale = 0.005, .noisiness = 6, .height = 256, .min_height = 320, .surface_block = Block::SNOW}, // mountain peak
	};

	(void)seed;
	chunkLocalVec3i	blockPos;
	for (blockPos.x() = 0; blockPos.x() < Chunk::SIZE; blockPos.x()++)
	{
		for (blockPos.z() = 0; blockPos.z() < Chunk::SIZE; blockPos.z()++)
		{ chunkWorldVec3i wp = chunkLocalToWorld(blockPos, chunk.pos(), Chunk::SIZE);

			float  continentalness = _noise.sample_fbm(vec2i(wp.x(), wp.z()), 0.00075, 6);

			Biome	b1 = biomes[0];
			Biome	b2 = biomes[0];
			Biome	b3 = biomes[0];

			auto biome_score = [](Biome b, float continentalness)
			{
				return (std::min(std::abs(b.continental_range.x() - continentalness), std::abs(b.continental_range.y() - continentalness)));
			};

			for (auto b : biomes)
			{
				float	score = biome_score(b, continentalness);
				if (score < biome_score(b1, continentalness))
				{
					b3 = b2;
					b2 = b1;
					b1 = b;
				}
			}

			for (blockPos.y() = 0; blockPos.y() < Chunk::SIZE; blockPos.y()++)
			{ chunkWorldVec3i wp = chunkLocalToWorld(blockPos, chunk.pos(), Chunk::SIZE);

				int	y = lerp(b1.min_height + _noise.sample_fbm(vec2i(wp.x(), wp.z()), b1.scale, b1.noisiness) * b1.height,
							lerp(b2.min_height + _noise.sample_fbm(vec2i(wp.x(), wp.z()), b2.scale, b2.noisiness) * b2.height,
								b3.min_height + _noise.sample_fbm(vec2i(wp.x(), wp.z()), b3.scale, b3.noisiness) * b3.height,
								1 - biome_score(b2, continentalness) - biome_score(b3, continentalness)),
							1 - biome_score(b1, continentalness) - biome_score(b2, continentalness));
				if (wp.y() <= y)
					chunk.setBlock(blockPos, b1.surface_block);
			}
		}
	}
	chunk.setState(Chunk::State::GENERATED);
}
