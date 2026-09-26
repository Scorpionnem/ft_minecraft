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
	if (!c)
		return ;

	_chunks.erase(h);
	_chunkRequests.erase(h);
	_chunkPool.release(c);
}

void	World::draw(const chunkWorldVec3i& center_chunk, u16 render_distance, const mbl::render::Camera& cam)
{
	render_distance /= 2;

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

void	World::requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, u32 max_new_requests)
{
	chunkWorldVec3i	pos;
	u32				sent = 0;
	render_distance /= 2;

	std::vector<chunkWorldVec3i>	requests;

	for (pos.x() = center_chunk.x() - render_distance; pos.x() <= center_chunk.x() + render_distance; pos.x()++)
		for (pos.y() = center_chunk.y() - render_distance; pos.y() <= center_chunk.y() + render_distance; pos.y()++)
			for (pos.z() = center_chunk.z() - render_distance; pos.z() <= center_chunk.z() + render_distance; pos.z()++)
			{
				requests.push_back(pos);
			}

	std::sort(requests.begin(), requests.end(), [center_chunk]
		(const chunkWorldVec3i& p1, const chunkWorldVec3i& p2)
		{
			return (vec3i::distance(p1, center_chunk) < vec3i::distance(p2, center_chunk));
		});

	for (const auto& p : requests)
	{
		if (requestChunk(p, net))
			sent++;
		if (sent > max_new_requests)
			break ;
	}
}

bool	World::requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net)
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

void	World::netChunkData(const Packet::ChunkData* pckt)
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

void	World::netChunkDataSpecial(const Packet::ChunkDataSpecial* pckt)
{
	chunkPosHash	h = hash(pckt->chunk_pos);
	auto			it = _chunkRequests.find(h);

	if (it == _chunkRequests.end())
		return ;

	PendingChunk&	pending = it->second;

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

void	World::generateInRange(const chunkWorldVec3i& center_chunk, u16 render_distance)
{
	render_distance /= 2;

	chunkWorldVec3i	pos;
	for (pos.x() = center_chunk.x() - render_distance; pos.x() <= center_chunk.x() + render_distance; pos.x()++)
		for (pos.y() = center_chunk.y() - render_distance; pos.y() <= center_chunk.y() + render_distance; pos.y()++)
			for (pos.z() = center_chunk.z() - render_distance; pos.z() <= center_chunk.z() + render_distance; pos.z()++)
				generateChunk(pos);
}

chunkPtr	World::generateChunk(const chunkWorldVec3i& pos)
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
