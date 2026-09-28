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
		u64	size()
		{
			return (_chunksPool.size());
		}
		u64	sizeFree()
		{
			return (_freeChunksPool.size());
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
			Block::BLUE_ORCHID = _blocks.registerBlock("blue_orchid", false, {}, "assets/models/blocks/blue_orchid.ftm")->getDefaultState().id();
			Block::STONE_SLAB = _blocks.registerBlock("blue_orchid", false, {BlockProperties::SLAB_POS}, "assets/models/blocks/stone_block.ftm")->getDefaultState().id();
			_blocks.computeBlocks();
		}

		void	setThreadPool(mbl::utils::ThreadPool* t)
		{
			_threads = t;
		}
		chunkPtr	getChunk(const chunkWorldVec3i& pos);

		void	meshChunk(chunkPtr c);

		void		draw(const chunkWorldVec3i& center_chunk, u16 render_distance, const mbl::render::Camera& cam, bool debug);

		void		generateInRange(const chunkWorldVec3i& center_chunk, u16 render_distance);
		chunkPtr	generateChunk(const chunkWorldVec3i& pos);

		chunkPtr	addChunk(const chunkWorldVec3i& pos);
		void		removeChunk(const chunkWorldVec3i& pos);

		bool	requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net, int& tx_pckt);
		void	requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, int& tx_pckt);

		void	netChunkData(const Packet::ChunkData* pckt);
		void	netChunkDataSpecial(const Packet::ChunkDataSpecial* pckt);

		void	clearUnused(const std::vector<chunkWorldVec3i>& centers, u16 render_distance)
		{
			render_distance /= 2;

			for (auto chunkIt = _chunks.begin(); chunkIt != _chunks.end();)
			{
				chunkPtr	chunk = chunkIt->second;

				bool	inRange = false;
				for (const vec3i& pos : centers)
				{
					vec3i diff = abs(pos - chunk->pos());
					if (diff.x() <= render_distance && diff.y() <= render_distance && diff.z() <= render_distance)
					{
						inRange = true;
						break ;
					}
				}

				if (!chunk->busy() && !_chunkRequests.contains(chunkIt->first) && !inRange)
				{
					_chunkPool.release(chunk);
					chunkIt = _chunks.erase(chunkIt);
				}
				else
				{
					chunkIt++;
				}
			}
		}

		u32	seed() {return (_seed);}
		void	setSeed(u32 seed) {_seed = seed;}

		void	setAtlas(mbl::render::TextureAtlas* atlas) {_atlas = atlas; _blocks.setAtlas(atlas);}
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

		mbl::render::TextureAtlas*	_atlas = nullptr;
};
