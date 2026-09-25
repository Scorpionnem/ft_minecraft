#pragma once

#include "mbl.hpp"
#include "game/world/World.hpp"

class	Server
{
	public:
		Server()
		{
			_running = false;
		}
		~Server() {}

		void    run(int port = 0)
        {
            init(port);
            loop();
        }
        void	stop() {_running = false;}
        bool	running() {return (_running);}

        const mbl::net::Server&	netServer() {return (_server);}
	private:
		void    init(int port);
        void    loop();

        void	update_broadcaster();
        void	update_server();
		void	_dispatch_packet(int fd, u8 *data, u64 size);
	private:
		std::map<int, u64>	_players;
		mbl::net::Server	_server;

		std::atomic_bool	_running;

		mbl::net::MulticastSender	broadcast;
		mbl::utils::Chrono  		broadcast_time;

		mbl::utils::ThreadPool	_threads;
		World					_world;
};
