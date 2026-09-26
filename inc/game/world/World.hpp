#pragma once

#include "game/world/Chunk.hpp"
#include "net/LAN.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>

using chunkPosHash = u64;
using chunkPtr = std::shared_ptr<Chunk>;

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
		World() {}
		~World() {}

		void	setThreadPool(mbl::utils::ThreadPool* t)
		{
			_threads = t;
		}
		chunkPtr	getChunk(const chunkWorldVec3i& pos);

		void		draw(const chunkWorldVec3i& center_chunk, u16 render_distance, const mbl::render::Camera& cam);

		void		generateInRange(const chunkWorldVec3i& center_chunk, u16 render_distance);
		chunkPtr	generateChunk(const chunkWorldVec3i& pos);

		chunkPtr	addChunk(const chunkWorldVec3i& pos);
		void		removeChunk(const chunkWorldVec3i& pos);

		bool	requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net);
		void	requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, u32 max_new_requests = 32);

		void	netChunkData(const Packet::ChunkData* pckt);
		void	netChunkDataSpecial(const Packet::ChunkDataSpecial* pckt);
	private:
		struct	PendingChunk
		{
			chunkPtr	chunk;
			u64			receivedMask = 0;
		};

		std::unordered_map<chunkPosHash, PendingChunk>	_chunkRequests;
		std::unordered_map<chunkPosHash, chunkPtr>		_chunks;
		ChunkPool										_chunkPool;

		mbl::utils::ThreadPool*						_threads = nullptr;
};
