#include "game/world/ClientWorld.hpp"
#include <bitset>

void	ClientWorld::update()
{
	std::vector<chunkWorldVec3i>	removes;

	for (auto& req : _chunkRequests)
		if (req.second.time.get() > CHUNK_REQUEST_TIMEOUT)
		{
			req.second.chunk->setBusy(false);
			removes.push_back(req.second.chunk->pos());
		}

	for (auto& p : removes)
		removeChunk(p);
}

void	ClientWorld::draw(const chunkWorldVec3i& center_chunk, u16 render_distance, const mbl::render::Camera& cam, bool debug, int octree_depth)
{
	render_distance /= 2;

	if (_atlas)
		_atlas->bind(0);

	for (auto& [h, chunk] : _chunks)
	{
		vec3i diff = abs(center_chunk - chunk->pos());

		if (!chunk->busy()
			&& chunk->state() >= Chunk::State::MESHED
			&& (diff.x() <= render_distance && diff.y() <= render_distance && diff.z() <= render_distance))
		{
			auto find = _meshes.find(hash(chunk->pos()));
			if (find == _meshes.end())
			{
				std::shared_ptr<ChunkRender>	chunk_render = std::make_shared<ChunkRender>(chunk->pos());
				_meshes.insert(std::make_pair(hash(chunk->pos()), chunk_render));
				if (chunk->non_air_blocks() != 0)
				{
					chunk_render->upload(chunk->meshData());
					root->appendNode(chunk_render);
				}
				continue ;
			}

			// find->second->draw(cam, debug);
		}
	}

	root->draw(cam, octree_depth);

	for (auto& [h, req] : _chunkRequests)
	{
		if (debug)
			mbl::render::renderer::AABBRenderer::draw(mbl::utils::aabb3f{.pos = req.chunk->pos() * Chunk::SIZE, .size = vec3f(Chunk::SIZE)}, cam, vec3f(req.time.get() / CHUNK_REQUEST_TIMEOUT, 0, 1 - req.time.get() / CHUNK_REQUEST_TIMEOUT));
	}
}

void	ClientWorld::requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, int& tx_pckt)
{
	render_distance /= 2;

	std::vector<chunkWorldVec3i>	requests;
	requests.reserve((2 * render_distance + 1) * (2 * render_distance + 1) * (2 * render_distance + 1));

	chunkWorldVec3i	pos;
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

bool	ClientWorld::requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net, int& tx_pckt)
{
	chunkPosHash	h = hash(pos);

	if (_chunkRequests.contains(h) || getChunk(pos) || _chunkRequests.size() >= MAX_CHUNK_REQUESTS)
		return (false);

	chunkPtr	c = addChunk(pos);
	c->setBusy(true);

	_chunkRequests.insert({h, PendingChunk{.chunk = c}});

	Packet::ChunkRequest	crq_pckt = {};
	crq_pckt.x = pos.x();
	crq_pckt.y = pos.y();
	crq_pckt.z = pos.z();

	tx_pckt++;
	net.send(&crq_pckt, sizeof(crq_pckt));
	return (true);
}

void	ClientWorld::meshChunk(chunkPtr c)
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
		if (neighbours[dir]->busy() || neighbours[dir]->state() < Chunk::State::GENERATED)
			neighbours[dir] = nullptr;
	}

	c->setBusy(true);
	auto		func = [this, c, neighbours]()
		{
			c->setMeshData(ChunkMesher::buildMesh(*c, neighbours, _blocks));
			c->setBusy(false);
		};

	if (_threads)
		_threads->queue_task(func);
	else
		func();
}

void	ClientWorld::_netChunkReceived(PendingChunk& pending)
{
	chunkPtr		c = pending.chunk;
	chunkPosHash	h = hash(c->pos());

	c->process_non_air();
	c->setState(Chunk::State::GENERATED);
	c->setBusy(false);
	if (c->empty())
		c->setState(Chunk::State::MESHED);
	else
		meshChunk(c);

	_chunkRequests.erase(h);
}

void	ClientWorld::netChunkData(const Packet::ChunkData* pckt)
{
	chunkWorldVec3i	pos = vec3i(pckt->x, pckt->y, pckt->z);
	chunkPosHash	h = hash(pos);
	auto			it = _chunkRequests.find(h);

	if (it == _chunkRequests.end() || pckt->id >= Chunk::PACKET_COUNT)
		return ;

	PendingChunk&	pending = it->second;
	u64				bit = 1ULL << pckt->id;

	if (pending.receivedMask & bit) // duplicate packet
		return ;

	for (size_t j = 0; j < Chunk::BLOCKS_PER_PACKET; ++j)
		pending.chunk->data()[j + pckt->id * Chunk::BLOCKS_PER_PACKET] = pckt->blocks[j];

	pending.receivedMask |= bit;

	pending.time.start();

	if (pending.receivedMask != (~0ULL >> (64 - Chunk::PACKET_COUNT))) // not fully received yet
		return ;

	chunkPtr	c = pending.chunk;

	_netChunkReceived(pending);
}

void	ClientWorld::netChunkDataSpecial(const Packet::ChunkDataSpecial* pckt)
{
	chunkPosHash	h = hash(vec3i(pckt->x, pckt->y, pckt->z));
	auto			it = _chunkRequests.find(h);

	if (it == _chunkRequests.end())
		return ;

	PendingChunk&	pending = it->second;
	chunkPtr	c = pending.chunk;

	if (pckt->type == Packet::ChunkDataSpecial::Type::EMPTY)
		_netChunkReceived(pending);
	else if (pckt->type == Packet::ChunkDataSpecial::Type::FAILURE)
	{
		c->setBusy(false);
		removeChunk(vec3i(pckt->x, pckt->y, pckt->z));
	}
}
