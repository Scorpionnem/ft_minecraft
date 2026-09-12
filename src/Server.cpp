#include "Server.hpp"

void    Server::init(int port)
{
	if (server.open(port) == -1)
		throw std::runtime_error("Failed to open server. (" + std::string(strerror(errno)) + ")");

	std::cout << "Server open: " << server.addr() << " " << server.port() << std::endl;

	running = true;
}

void    Server::loop()
{
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
