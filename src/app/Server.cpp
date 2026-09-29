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
	_world.loadBlocks();
	_world.setThreadPool(&_threads);
	_world.setSeed(Noise::rand1dTo1d(_server.port()) * (double)UINT32_MAX);
}

void    Server::loop()
{
	mbl::utils::Chrono	c;
	while (_running)
	{
		update_server();

		if (c.get() > (1.0f / 20.0f))
		{
			_world.update(_server);
			std::vector<chunkWorldVec3i>	positions;
			for (auto& [fd, pos] : _playersPos)
				positions.push_back(worldToChunkWorld(pos, Chunk::SIZE));
			_world.clearUnused(positions, RENDER_DISTANCE);
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

	mbl::net::Server::Event	event = mbl::net::Server::NONE;
	u8					buf[4096] = {};
	u64					size = 0;
	int					fd = 0;

	do
	{
		if (_server.recv(buf, sizeof(buf), event, size, fd) == -1)
		{
			perror("recv");
			_running = false;
			break ;
		}

		if (event == mbl::net::Server::Event::CONNECTION)
		{
		}
		else if (event == mbl::net::Server::Event::DISCONNECT)
		{
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
		case CHUNKREQUEST_TYPE:
		{
			Packet::ChunkRequest*	req_pckt = reinterpret_cast<Packet::ChunkRequest*>(data);

			_world.clientChunkRequest(fd, _server, req_pckt, worldToChunkWorld(_playersPos[fd], Chunk::SIZE), RENDER_DISTANCE);
			break ;
		}
		case PLAYERPOS_TYPE:
		{
			Packet::PlayerPos*	pos_pckt = reinterpret_cast<Packet::PlayerPos*>(data);

			_playersPos[fd] = vec3f(pos_pckt->x, pos_pckt->y, pos_pckt->z);
			break ;
		}
		default :
			return ;
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
