#pragma once

#include "game/world/Chunk.hpp"
#include "game/world/Block.hpp"
#include "game/world/generation/ChunkGenerator.hpp"
#include "game/world/generation/NoiseGenerator.hpp"
#include "game/world/generation/OverworldGenerator.hpp"
#include "game/world/render/mesh/ChunkMesher.hpp"
#include "game/world/ChunkPool.hpp"
#include "net/LAN.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <bitset>

class	World
{
	public:
		static constexpr int	MAX_CHUNK_REQUESTS = 256;
	public:
		virtual ~World() {}

		void	loadBlocks()
		{
			Block::AIR = _blocks.registerBlock("air", false)->getDefaultState().id();
			Block::STONE = _blocks.registerBlock("stone", true, {}, "assets/models/blocks/stone_block.ftm")->getDefaultState().id();
			Block::GRASS_BLOCK = _blocks.registerBlock("grass_block", true, {}, "assets/models/blocks/grass_block.ftm")->getDefaultState().id();
			Block::GRASS = _blocks.registerBlock("grass", false, {}, "assets/models/blocks/grass.ftm")->getDefaultState().id();
			Block::BLUE_ORCHID = _blocks.registerBlock("blue_orchid", false, {}, "assets/models/blocks/blue_orchid.ftm")->getDefaultState().id();
			Block::STONE_SLAB = _blocks.registerBlock("blue_orchid", false, {BlockProperties::SLAB_POS}, "assets/models/blocks/stone_block.ftm")->getDefaultState().id();
			Block::SNOW = _blocks.registerBlock("snow", true, {}, "assets/models/blocks/snow_block.ftm")->getDefaultState().id();
			Block::MOSS = _blocks.registerBlock("moss", true, {}, "assets/models/blocks/moss_block.ftm")->getDefaultState().id();
			Block::COBBLESTONE = _blocks.registerBlock("cobblestone", true, {}, "assets/models/blocks/cobblestone_block.ftm")->getDefaultState().id();
			Block::SAND = _blocks.registerBlock("sand", true, {}, "assets/models/blocks/sand_block.ftm")->getDefaultState().id();
			_blocks.computeBlocks();
		}

		chunkPtr		getChunk(const chunkWorldVec3i& pos);
		chunkPtr		addChunk(const chunkWorldVec3i& pos);
		virtual std::unordered_map<chunkPosHash, chunkPtr>::iterator	removeChunk(const chunkWorldVec3i& pos);

		void		clearUnused(const std::vector<chunkWorldVec3i>& centers, u16 render_distance);

		BlockRegistry&	getBlocks() {return (_blocks);}
		u64				chunks() {return (_chunks.size());}
		void			setThreadPool(mbl::utils::ThreadPool* t) {_threads = t;}
	protected:

		u32	_seed = 0;

		std::unordered_map<chunkPosHash, chunkPtr>		_chunks;
		ChunkPool										_chunkPool;

		mbl::utils::ThreadPool*						_threads = nullptr;

		BlockRegistry		_blocks;
};
