#include "app/Server.hpp"
#include "net/LAN.hpp"
#include "game/world/World.hpp"

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

void    Server::init(int port)
{
	if (broadcast.open("224.0.0.42", 6767) == -1)
		throw std::runtime_error(std::string(strerror(errno)));

	if (_server.open(port) == -1)
		throw std::runtime_error("Failed to open server. (" + std::string(strerror(errno)) + ")");

	_running = true;

	_threads.add(16);
}

void    Server::loop()
{
	mbl::utils::Chrono	c;
	while (_running)
	{
		update_server();

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
		case ENTITYPOS_TYPE:
		{
			Packet::EntityPos*	pos_pckt = reinterpret_cast<Packet::EntityPos*>(data);

			_world.generateInRange(worldToChunkWorld(pos_pckt->pos, Chunk::SIZE), 3);

			_server.send_all_except(fd, pos_pckt, sizeof(*pos_pckt));
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
