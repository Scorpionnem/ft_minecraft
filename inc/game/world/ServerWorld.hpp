#pragma once

#include "game/world/World.hpp"

class	ServerWorld : public World
{
	public:
		ServerWorld()
		{
			_generator = std::make_shared<OverworldGenerator>();
		}

		void	update(mbl::net::Server &serv)
		{
			_service_pending_chunk_sends(serv);
		}

		void		generateInRange(const chunkWorldVec3i& center_chunk, u16 render_distance);
		chunkPtr	generateChunk(const chunkWorldVec3i& pos);

		u32		seed() {return (_seed);}
		void	setSeed(u32 seed) {_seed = seed; _generator->init(_seed);}

		void	setGenerator(std::shared_ptr<ChunkGenerator> gen) {_generator = gen;}
		std::shared_ptr<ChunkGenerator>	generator() {return (_generator);}

		void	clientChunkRequest(int fd, mbl::net::Server &serv, Packet::ChunkRequest* pckt, const chunkWorldVec3i& player_pos, u16 render_distance)
		{
			vec3i diff = abs(player_pos - vec3i(pckt->x, pckt->y, pckt->z));
			if (!(diff.x() <= render_distance && diff.y() <= render_distance && diff.z() <= render_distance))
			{
				Packet::ChunkDataSpecial	ret_err = {};

				ret_err.x = pckt->x;
				ret_err.y = pckt->y;
				ret_err.z = pckt->z;
				ret_err.type = Packet::ChunkDataSpecial::Type::FAILURE;
				serv.send(fd, &ret_err, sizeof(ret_err));
				return ;
			}

			chunkPtr	c = generateChunk(vec3i(pckt->x, pckt->y, pckt->z));

			if (c->busy())
				_pendingChunkSends.push_back(PendingChunkSend{.fd = fd, .chunk = c});
			else
				_send_chunk(fd, serv, c);
		}
	private:
		std::shared_ptr<ChunkGenerator>	_generator;

		struct	PendingChunkSend
		{
			int			fd;
			chunkPtr	chunk;
		};
		std::vector<PendingChunkSend>	_pendingChunkSends;
		void	_send_chunk(int fd, mbl::net::Server &serv, chunkPtr chunk)
		{
			chunkWorldVec3i	pos = chunk->pos();

			if (chunk->empty())
			{
				Packet::ChunkDataSpecial	pckt = {};
				pckt.x = pos.x();
				pckt.y = pos.y();
				pckt.z = pos.z();
				pckt.type = Packet::ChunkDataSpecial::Type::EMPTY;
				serv.send(fd, &pckt, sizeof(pckt));
				return ;
			}

			for (u32 i = 0; i < Chunk::PACKET_COUNT; i++)
			{
				Packet::ChunkData	pckt = {};

				pckt.x = pos.x();
				pckt.y = pos.y();
				pckt.z = pos.z();
				pckt.id = i;

				for (size_t j = 0; j < Chunk::BLOCKS_PER_PACKET; ++j)
					pckt.blocks[j] = chunk->data()[i * Chunk::BLOCKS_PER_PACKET + j];

				serv.send(fd, &pckt, sizeof(pckt));
			}
		}
		void	_service_pending_chunk_sends(mbl::net::Server &serv)
		{
			auto	it = _pendingChunkSends.begin();

			while (it != _pendingChunkSends.end())
			{
				if (it->chunk->busy())
				{
					++it;
					continue ;
				}

				_send_chunk(it->fd, serv, it->chunk);
				it = _pendingChunkSends.erase(it);
			}
		}
};
