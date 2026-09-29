#include "game/world/ServerWorld.hpp"

void	ServerWorld::generateInRange(const chunkWorldVec3i& center_chunk, u16 render_distance)
{
	render_distance /= 2;

	chunkWorldVec3i	pos;
	for (pos.x() = center_chunk.x() - render_distance; pos.x() <= center_chunk.x() + render_distance; pos.x()++)
		for (pos.y() = center_chunk.y() - render_distance; pos.y() <= center_chunk.y() + render_distance; pos.y()++)
			for (pos.z() = center_chunk.z() - render_distance; pos.z() <= center_chunk.z() + render_distance; pos.z()++)
				generateChunk(pos);
}

chunkPtr	ServerWorld::generateChunk(const chunkWorldVec3i& pos)
{
	chunkPtr gc = getChunk(pos);
	if (gc)
		return (gc);

	chunkPtr		c = addChunk(pos);

	c->setBusy(true);
	auto func = [this, c]()
		{
			if (_generator)
				_generator->generateChunk(*c, seed());
			c->setBusy(false);
		};

	if (_threads)
		_threads->queue_task(func);
	else
		func();

	return (c);
}
