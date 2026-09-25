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
				return (c);
			}
			chunkPtr	c = std::make_shared<Chunk>();
			_chunksPool.push_back(c);
			return (c);
		}
		void	release(chunkPtr c)
		{
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
		chunkPtr	getChunk(const chunkWorldVec3i& pos)
		{
			chunkPosHash	h = hash(pos);
			auto find = _chunks.find(h);
			if (find == _chunks.end())
				return (nullptr);

			return (find->second);
		}
		void	draw(const chunkWorldVec3i& center_chunk, u16 render_distance, const mbl::render::Camera& cam)
		{
			chunkWorldVec3i	pos;
			for (pos.x() = center_chunk.x() - render_distance; pos.x() <= center_chunk.x() + render_distance; pos.x()++)
				for (pos.y() = center_chunk.y() - render_distance; pos.y() <= center_chunk.y() + render_distance; pos.y()++)
					for (pos.z() = center_chunk.z() - render_distance; pos.z() <= center_chunk.z() + render_distance; pos.z()++)
					{
						chunkPtr c = getChunk(pos);
						if (c && !c->busy())
						{
							c->draw(cam);
						}
					}
		}
		void	generateInRange(const chunkWorldVec3i& center_chunk, u16 render_distance)
		{
			chunkWorldVec3i	pos;
			for (pos.x() = center_chunk.x() - render_distance; pos.x() <= center_chunk.x() + render_distance; pos.x()++)
				for (pos.y() = center_chunk.y() - render_distance; pos.y() <= center_chunk.y() + render_distance; pos.y()++)
					for (pos.z() = center_chunk.z() - render_distance; pos.z() <= center_chunk.z() + render_distance; pos.z()++)
						generateChunk(pos);
		}
		chunkPtr	generateChunk(const chunkWorldVec3i& pos)
		{
			chunkPtr gc = getChunk(pos);
			if (gc)
				return (gc);

			chunkPosHash	h = hash(pos);
			chunkPtr		c = _chunkPool.get();

			c->setPos(pos);

			c->setBusy(true);
			auto func = [c]()
				{
					c->generate();
					c->setBusy(false);
				};

			if (_threads)
				_threads->queue_task(func);
			else
				func();

			_chunks.insert({h, c});
			return (c);
		}

		chunkPtr	addChunk(const chunkWorldVec3i& pos)
		{
			chunkPosHash	h = hash(pos);
			chunkPtr		c = _chunkPool.get();

			c->setPos(pos);

			_chunks.insert({h, c});
			return (c);
		}

		bool	requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net)
		{
			chunkPosHash	h = hash(pos);

			if (_chunkRequests.contains(h) || getChunk(pos))
				return (false);

			chunkPtr	c = addChunk(pos);
			c->setBusy(true);
			_chunkRequests.insert({h, PendingChunk{.chunk = c}});

			Packet::ChunkRequest	crq_pckt = {};

			crq_pckt.chunk_pos = pos;
			net.send(&crq_pckt, sizeof(crq_pckt));
			return (true);
		}
		void	requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, u32 max_new_requests = 8)
		{
			chunkWorldVec3i	pos;
			u32				sent = 0;

			for (pos.x() = center_chunk.x() - render_distance; pos.x() <= center_chunk.x() + render_distance && sent < max_new_requests; pos.x()++)
				for (pos.y() = center_chunk.y() - render_distance; pos.y() <= center_chunk.y() + render_distance && sent < max_new_requests; pos.y()++)
					for (pos.z() = center_chunk.z() - render_distance; pos.z() <= center_chunk.z() + render_distance && sent < max_new_requests; pos.z()++)
						if (requestChunk(pos, net))
							sent++;
		}
		void	netChunkData(const Packet::ChunkData* pckt)
		{
			chunkPosHash	h = hash(pckt->chunk_pos);
			auto			it = _chunkRequests.find(h);

			if (it == _chunkRequests.end() || pckt->id >= Chunk::PACKET_COUNT)
				return ;

			PendingChunk&	pending = it->second;
			u64				bit = 1ULL << pckt->id;

			if (pending.receivedMask & bit)
				return ;

			std::copy(pckt->blocks, pckt->blocks + Chunk::BLOCKS_PER_PACKET, pending.chunk->data().begin() + pckt->id * Chunk::BLOCKS_PER_PACKET);
			pending.receivedMask |= bit;

			if (pending.receivedMask != (~0ULL >> (64 - Chunk::PACKET_COUNT)))
				return ;

			chunkPtr	c = pending.chunk;
			auto		func = [c]()
				{
					c->mesh();
					c->setBusy(false);
				};

			if (_threads)
				_threads->queue_task(func);
			else
				func();

			_chunkRequests.erase(it);
		}
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
