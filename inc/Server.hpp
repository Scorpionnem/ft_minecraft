#pragma once

#include "mbl.hpp"

class	Server
{
	public:
		~Server() {}

		void    run(int port)
        {
            init(port);
            loop();
        }
	private:
		void    init(int port);
        void    loop();
	private:
		mbl::net::Server	server;

		bool	running = false;
};
