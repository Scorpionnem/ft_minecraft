#pragma once

#include "game/world/Chunk.hpp"

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

		void	shrink()
		{
			for (auto& c : _freeChunksPool)
			{
				auto f = std::find(_chunksPool.begin(), _chunksPool.end(), c);
				if (f != _chunksPool.end())
					_chunksPool.erase(f);
			}
			_freeChunksPool.clear();
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
