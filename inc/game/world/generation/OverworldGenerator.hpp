#pragma once

#include "game/world/generation/ChunkGenerator.hpp"
#include "game/world/generation/noise/Noise.hpp"

class OverworldGenerator : public ChunkGenerator
{
	public:
		OverworldGenerator() {};
		~OverworldGenerator() {};

		void	init(u32 seed)
		{
			_seed = seed;

			_noise.init(_seed);
		}
		void generateChunk(Chunk& chunk, u32 seed);
	private:
		u32	_seed;
		Noise::Perlin2D	_noise;
};
