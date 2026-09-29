#include "game/world/World.hpp"

chunkPtr	World::getChunk(const chunkWorldVec3i& pos)
{
	chunkPosHash	h = hash(pos);
	auto find = _chunks.find(h);
	if (find == _chunks.end())
		return (nullptr);

	return (find->second);
}

chunkPtr	World::addChunk(const chunkWorldVec3i& pos)
{
	chunkPosHash	h = hash(pos);
	chunkPtr		c = _chunkPool.get();

	c->setPos(pos);

	_chunks.insert({h, c});
	return (c);
}

void	World::removeChunk(const chunkWorldVec3i& pos)
{
	chunkPosHash	h = hash(pos);
	chunkPtr	c = getChunk(pos);
	if (!c || c->busy())
		return ;

	_chunks.erase(h);
	_chunkPool.release(c);
}

void	World::clearUnused(const std::vector<chunkWorldVec3i>& centers, u16 render_distance)
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

		if (!chunk->busy() && !inRange)
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
