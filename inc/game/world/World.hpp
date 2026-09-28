#pragma once

#include "game/world/Chunk.hpp"
#include "game/world/Block.hpp"
#include "net/LAN.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>

class	ChunkPool
{
	public:
		ChunkPool() {}
		~ChunkPool() {}

		chunkPtr	get()
		{
			if (_freeChunksPool.size())
			{
				chunkPtr c = _freeChunksPool.back();
				_freeChunksPool.pop_back();
				c->clear();
				return (c);
			}
			chunkPtr	c = std::make_shared<Chunk>();
			_chunksPool.push_back(c);
			return (c);
		}
		void	release(chunkPtr c)
		{
			c->clear();
			_freeChunksPool.push_back(c);
		}
	private:
		std::vector<chunkPtr>							_chunksPool;
		std::vector<chunkPtr>							_freeChunksPool;
};

class	World
{
	public:
		static constexpr int	MAX_CHUNK_REQUESTS = 64;
	public:
		World() {}
		~World() {}

		void	loadBlocks()
		{
			Block::AIR = _blocks.registerBlock("air", false)->getDefaultState().id();
			Block::STONE = _blocks.registerBlock("stone", true, {}, "assets/models/blocks/stone_block.ftm")->getDefaultState().id();
			Block::GRASS_BLOCK = _blocks.registerBlock("grass_block", true, {}, "assets/models/blocks/grass_block.ftm")->getDefaultState().id();
			Block::GRASS = _blocks.registerBlock("grass", false, {}, "assets/models/blocks/grass.ftm")->getDefaultState().id();
		}

		void	setThreadPool(mbl::utils::ThreadPool* t)
		{
			_threads = t;
		}
		chunkPtr	getChunk(const chunkWorldVec3i& pos);

		void	meshChunk(chunkPtr c);

		void		draw(const chunkWorldVec3i& center_chunk, u16 render_distance, const mbl::render::Camera& cam);

		void		generateInRange(const chunkWorldVec3i& center_chunk, u16 render_distance);
		chunkPtr	generateChunk(const chunkWorldVec3i& pos);

		chunkPtr	addChunk(const chunkWorldVec3i& pos);
		void		removeChunk(const chunkWorldVec3i& pos);

		bool	requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net, int& tx_pckt);
		void	requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, int& tx_pckt);

		void	netChunkData(const Packet::ChunkData* pckt);
		void	netChunkDataSpecial(const Packet::ChunkDataSpecial* pckt);

		u32	seed() {return (_seed);}
		void	setSeed(u32 seed) {_seed = seed;}
	private:
		struct	PendingChunk
		{
			chunkPtr	chunk;
			u64			receivedMask = 0;
		};

		u32	_seed = 0;

		std::unordered_map<chunkPosHash, PendingChunk>	_chunkRequests;
		std::unordered_map<chunkPosHash, chunkPtr>		_chunks;
		ChunkPool										_chunkPool;

		mbl::utils::ThreadPool*						_threads = nullptr;

		BlockRegistry		_blocks;
};
