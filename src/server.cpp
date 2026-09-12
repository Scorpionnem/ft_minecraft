#include "net/Server.hpp"

#include <iostream>

#include "net/Packet.hpp"
#include "utils/Chrono.hpp"

int	main(void)
{
	mbl::utils::Chrono	c;
	mbl::net::Server	server;

	if (server.open(6767) == -1)
		return 1;

	std::cout << server.addr() << ":" << server.port() << std::endl;

	bool	running = true;
	while (running)
	{
		if (server.update() == -1)
			break ;

		mbl::net::Server::Event	event;
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

			if (event == mbl::net::Server::Event::CONNECTION)
				std::cout << "client " << fd << " connected" << std::endl;
			else if (event == mbl::net::Server::Event::DISCONNECT)
				std::cout << "client " << fd << " disconnected" << std::endl;
			else if (event == mbl::net::Server::Event::RECV)
			{
				mbl::net::Packet::Header	*hdr = reinterpret_cast<mbl::net::Packet::Header*>(buf);
				switch (hdr->type)
				{
					case 2:
					{
						mbl::net::Packet::RTTReply	repl;
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
				mbl::net::Packet::Position	pos;
				pos.x = rand() % 32;
				pos.y = rand() % 32;
				pos.z = rand() % 32;
				server.send_all(&pos, sizeof(pos));
			}
		} while (event != mbl::net::Server::Event::NONE);
	}
	server.close();
}
