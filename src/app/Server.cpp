#include "app/Server.hpp"
#include "net/LAN.hpp"

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

void    Server::init(int port)
{
	if (broadcast.open("224.0.0.42", 6767) == -1)
		throw std::runtime_error(std::string(strerror(errno)));

	if (_server.open(port) == -1)
		throw std::runtime_error("Failed to open server. (" + std::string(strerror(errno)) + ")");

	std::cout << "Server open: " << _server.addr() << " " << _server.port() << std::endl;

	_running = true;
}

void    Server::loop()
{
	mbl::utils::Chrono	c;
	while (_running)
	{
		update_server();

		if (c.get() > 0.05)
		{
			for (const auto& en : entities.get_all())
			{
				Packet::EntityPos	pos_pckt = {};

				pos_pckt.pos = en.second.pos;
				pos_pckt.yaw = en.second.yaw;
				pos_pckt.pitch = en.second.pitch;
				pos_pckt.id = en.second.id;
				_server.send_all(&pos_pckt, sizeof(pos_pckt));
			}
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
			Entity	tmp_en = {};
			Entity*	en = entities.add(tmp_en);
			Packet::EntityPos	en_pckt = {};
			en_pckt.id = en->id;
			_players[fd] = en->id;
			_server.send(fd, &en_pckt, sizeof(en_pckt));
		}
		else if (event == mbl::net::Server::Event::DISCONNECT)
		{
			Entity*	en = entities.get(_players[fd]);
			if (!en)
				return ;
			Packet::EntityRemove	rm_pckt;
			rm_pckt.id = en->id;
			_server.send(fd, &rm_pckt, sizeof(rm_pckt));
		}
		else if (event == mbl::net::Server::Event::RECV)
		{
			_dispatch_packet(fd, buf, size);
			//std::cout << "recv from " << fd << std::endl;
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
			Packet::EntityPos*	en_pckt = reinterpret_cast<Packet::EntityPos*>(data);

			Entity*	en = entities.get(_players[fd]);
			en->pos = en_pckt->pos;
			en->yaw = en_pckt->yaw;
			en->pitch = en_pckt->pitch;
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
