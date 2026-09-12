#include "Client.hpp"
#include "Server.hpp"

int main(int ac, char **av)
{
    if (ac < 2)
    {
    	std::cerr << "Usage: ./ft_minecraft client|server" << std::endl;
    	return (1);
    }

    std::string cmd = av[1];

    try
    {
	    if (cmd == "client")
	    {
			if (ac != 4)
			{
				std::cerr << "Usage: ./ft_minecraft client ip port" << std::endl;
				return (1);
			}

    		std::string	ip = av[2];
      		int			port = std::atoi(av[3]);

			Client client;

			client.run(ip, port);
	    }
	    else if (cmd == "server")
	    {
			if (ac != 3)
			{
				std::cerr << "Usage: ./ft_minecraft server port" << std::endl;
				return (1);
			}

			int			port = std::atoi(av[2]);

			Server server;

			server.run(port);
	    }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
