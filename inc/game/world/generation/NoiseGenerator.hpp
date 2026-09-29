#pragma once

#include "game/world/generation/ChunkGenerator.hpp"
#include "game/world/generation/noise/Noise.hpp"

class NoiseGenerator : public ChunkGenerator
{
	public:
		NoiseGenerator() {};
		~NoiseGenerator() {};

		void generateChunk(Chunk& chunk, u32 seed);
};
