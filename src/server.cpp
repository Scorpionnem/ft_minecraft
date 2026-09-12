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
				std::cout << "recv" << std::endl;
			}

		} while (event != mbl::net::Server::Event::NONE);
	}
	server.close();
}
