#pragma once

#include "mbl.hpp"

class Chunk;

class ChunkGenerator
{
	public:
		virtual ~ChunkGenerator() {};
		virtual void generateChunk(Chunk& chunk, u32 seed) = 0;
};
