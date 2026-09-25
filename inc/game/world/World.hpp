#pragma once

#include "game/world/Chunk.hpp"
#include "net/LAN.hpp"
#include <memory>

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
							c->draw(cam, true);
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
					c->mesh();
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

			_chunks.insert({h, c});
			return (c);
		}

		void	requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net)
		{
			chunkPosHash	h = hash(pos);

			if (_chunkRequests.contains(h))
				return ;

			chunkPtr	c = addChunk(pos);
			c->setBusy(true);
			_chunkRequests.insert({h, c});

			Packet::ChunkRequest	crq_pclt = {};

			crq_pclt.chunk_pos = pos;
			net.send(&crq_pclt, sizeof(crq_pclt));
		}
		void	netChunkData(const Packet::ChunkData* pckt)
		{
			chunkPosHash	h = hash(pckt->chunk_pos);

			if (!_chunkRequests.contains(h))
				return ;

			chunkPtr c = _chunkRequests.find(h)->second;

			memcpy(c->data().data() + pckt->id * 64, pckt->blocks, sizeof(pckt->blocks));
		}
	private:

		std::unordered_map<chunkPosHash, chunkPtr>	_chunkRequests;
		std::unordered_map<chunkPosHash, chunkPtr>	_chunks;
		ChunkPool									_chunkPool;

		mbl::utils::ThreadPool*						_threads = nullptr;
};
