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

void	World::requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, int& tx_pckt)
{
	chunkWorldVec3i	pos;
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
		requestChunk(p, net, tx_pckt);
}

bool	World::requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net, int& tx_pckt)
{
	chunkPosHash	h = hash(pos);

	if (_chunkRequests.contains(h) || getChunk(pos) || _chunkRequests.size() >= MAX_CHUNK_REQUESTS)
		return (false);

	chunkPtr	c = addChunk(pos);
	c->setBusy(true);
	_chunkRequests.insert({h, PendingChunk{.chunk = c}});

	Packet::ChunkRequest	crq_pckt = {};

	crq_pckt.chunk_pos = pos;
	tx_pckt++;
	net.send(&crq_pckt, sizeof(crq_pckt));
	return (true);
}

void	World::meshChunk(chunkPtr c)
{
	if (!c)
		return ;

	chunkWorldVec3i	pos = c->pos();

	std::array<chunkPtr, 6>	neighbours = {
		getChunk(vec3i(pos.x(), pos.y() + 1, pos.z())),
		getChunk(vec3i(pos.x(), pos.y() - 1, pos.z())),
		getChunk(vec3i(pos.x(), pos.y(), pos.z() + 1)),
		getChunk(vec3i(pos.x(), pos.y(), pos.z() - 1)),
		getChunk(vec3i(pos.x() + 1, pos.y(), pos.z())),
		getChunk(vec3i(pos.x() - 1, pos.y(), pos.z())),
	};

	for (int dir = 0; dir < 6; dir++)
	{
		if (!neighbours[dir])
			continue ;
		if (neighbours[dir]->state() < Chunk::State::GENERATED)
			neighbours[dir] = nullptr;
	}

	auto		func = [c, neighbours]()
		{
			c->mesh(neighbours);
			c->setBusy(false);
		};

	if (_threads)
		_threads->queue_task(func);
	else
		func();
}

void	World::netChunkData(const Packet::ChunkData* pckt)
{
	chunkWorldVec3i	pos = pckt->chunk_pos;
	chunkPosHash	h = hash(pos);
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

	meshChunk(c);

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

	meshChunk(c);

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
	u32	s = seed();
	auto func = [c, s]()
		{
			c->generate(s);
			c->setBusy(false);
		};

	if (_threads)
		_threads->queue_task(func);
	else
		func();

	_chunks.insert({h, c});
	return (c);
}
