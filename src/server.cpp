#include "net/Server.hpp"

#include <iostream>

#include "net/Packet.hpp"
#include "utils/Chrono.hpp"

int	main(void)
{
	Chrono	c;
	net::Server	server;

	if (server.open(6767) == -1)
		return 1;

	std::cout << server.addr() << ":" << server.port() << std::endl;

	bool	running = true;
	while (running)
	{
		if (server.update() == -1)
			break ;

		net::Server::Event	event;
		u8					buf[4096];
		u64					size;
		int					fd;

		do
		{
			if (server.recv(buf, sizeof(buf), event, size, fd) == -1)
			{
				running = false;
				break ;
			}

			if (event == net::Server::Event::CONNECTION)
				std::cout << "client " << fd << " connected" << std::endl;
			else if (event == net::Server::Event::DISCONNECT)
				std::cout << "client " << fd << " disconnected" << std::endl;
			else if (event == net::Server::Event::RECV)
			{
				Packet::Header	*hdr = reinterpret_cast<Packet::Header*>(buf);
				switch (hdr->type)
				{
					case 2:
					{
						Packet::RTTReply	repl;
						server.send(fd, &repl, sizeof(repl));
						break ;
					}
					default:
						break ;
				}
			}

			if (c.get() > 0.3)
			{
				c.start();
				Packet::Position	pos;
				pos.x = rand() % 32;
				pos.y = rand() % 32;
				pos.z = rand() % 32;
				server.send_all(&pos, sizeof(pos));
			}
		} while (event != net::Server::Event::NONE);
	}
	server.close();
}
