#include "mbl.hpp"

#include "app/Client.hpp"
#include "app/Server.hpp"

int	main(int ac, char **av)
{
	try
	{
		if (ac == 1 || (ac >= 2 && std::string(av[1]) == "client"))
		{
			Client	client;

			client.run();
		}
		else if ((ac >= 2 && std::string(av[1]) == "server"))
		{
			Server	server;

			int	port = ac == 3 ? std::atoi(av[2]) : 6767;

			server.run(port);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
