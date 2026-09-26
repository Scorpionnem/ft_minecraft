#include "app/Server.hpp"
#include "net/LAN.hpp"
#include "game/world/World.hpp"

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <algorithm>

void    Server::init(int port)
{
	if (broadcast.open("224.0.0.42", 6767) == -1)
		throw std::runtime_error(std::string(strerror(errno)));

	if (_server.open(port) == -1)
		throw std::runtime_error("Failed to open server. (" + std::string(strerror(errno)) + ")");

	_running = true;

	_threads.add(16);
	_world.setThreadPool(&_threads);
}

void    Server::loop()
{
	mbl::utils::Chrono	c;
	while (_running)
	{
		update_server();
		_service_pending_chunk_sends();

		if (c.get() > (1.0f / 20.0f))
		{
			c.start();
		}

		update_broadcaster();
	}
	_server.close();
	_running = false;
}

static int	get_local_ip(char* buf, u64 size)
{
	char	host[256];
	if (gethostname(host, sizeof(host)) == -1)
        return (-1);

    hostent*	he = gethostbyname(host);
    if (!he)
        return (-1);

    in_addr*	addr = (in_addr*)he->h_addr_list[0];
    char*		s = inet_ntoa(*addr);
    memcpy(buf, s, std::min(strlen(s), size));

    return (0);
}

void	Server::update_server()
{
	if (_server.update() == -1)
		return ;

	mbl::net::Server::Event	event;
	u8					buf[4096];
	u64					size;
	int					fd;

	do
	{
		if (_server.recv(buf, sizeof(buf), event, size, fd) == -1)
		{
			_running = false;
			break ;
		}

		if (event == mbl::net::Server::Event::CONNECTION)
		{
			_player_entities.insert({fd, _entities.add(Player())});
		}
		else if (event == mbl::net::Server::Event::DISCONNECT)
		{
			_entities.remove(_player_entities[fd]);
			_player_entities.erase(fd);
		}
		else if (event == mbl::net::Server::Event::RECV)
		{
			_dispatch_packet(fd, buf, size);
		}

	} while (event != mbl::net::Server::Event::NONE);
}

void	Server::_dispatch_packet(int fd, u8 *data, u64 size)
{
	if (size < sizeof(Packet::Header))
		return ;

	Packet::Header*	hdr = reinterpret_cast<Packet::Header*>(data);
	if (hdr->magic != MINECRAFT_PCKT_MAGIC) // invalid packet
		return ;

	switch (hdr->type)
	{
		case ENTITYINFO_TYPE:
		{
			Packet::EntityInfo*	pos_pckt = reinterpret_cast<Packet::EntityInfo*>(data);

			Entity& en = _entities.get(_player_entities[fd]);

			en.pos = pos_pckt->entity.pos;
			en.yaw = pos_pckt->entity.yaw;
			en.pitch = pos_pckt->entity.pitch;
			Packet::EntityInfo	enpckt = {};
			enpckt.entity = en;
			enpckt.id = en.id;
			_server.send_all_except(fd, &enpckt, sizeof(enpckt));
			break ;
		}
		case CHUNKREQUEST_TYPE:
		{
			Packet::ChunkRequest*	req_pckt = reinterpret_cast<Packet::ChunkRequest*>(data);
			chunkPtr				chunk = _world.generateChunk(req_pckt->chunk_pos);

			if (chunk->busy())
				_pendingChunkSends.push_back({fd, chunk});
			else
				_send_chunk(fd, chunk);
			break ;
		}
		default :
			return ;
	}
}

void	Server::_send_chunk(int fd, chunkPtr chunk)
{
	if (chunk->empty())
	{
		Packet::ChunkDataSpecial	sp = {};

		sp.type = Packet::ChunkDataSpecial::Type::EMPTY;
		sp.chunk_pos = chunk->pos();
		_server.send(fd, &sp, sizeof(sp));
		return ;
	}

	chunkWorldVec3i	pos = chunk->pos();

	for (u32 i = 0; i < Chunk::PACKET_COUNT; i++)
	{
		Packet::ChunkData	pckt = {};

		pckt.chunk_pos = pos;
		pckt.id = i;
		std::copy(chunk->data().begin() + i * Chunk::BLOCKS_PER_PACKET, chunk->data().begin() + (i + 1) * Chunk::BLOCKS_PER_PACKET, pckt.blocks);

		_server.send(fd, &pckt, sizeof(pckt));
	}
}

void	Server::_service_pending_chunk_sends()
{
	auto	it = _pendingChunkSends.begin();

	while (it != _pendingChunkSends.end())
	{
		if (it->chunk->busy())
		{
			++it;
			continue ;
		}

		_send_chunk(it->fd, it->chunk);
		it = _pendingChunkSends.erase(it);
	}
}

void	Server::update_broadcaster()
{
	#define BROADCAST_DELAY (0.5)
	if (broadcast_time.get() > BROADCAST_DELAY)
	{
		Packet::LANBroadcast	packet = {};

		get_local_ip(packet.addr, sizeof(packet.addr));
		packet.port = _server.port();

		if (broadcast.send(&packet, sizeof(packet)) == -1)
			throw std::runtime_error(std::string(strerror(errno)));
		broadcast_time.start();
	}
}
