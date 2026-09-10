#pragma once

#include <vector>
#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <stdbool.h>
#include <string>

using u8 = uint8_t;
using u64 = uint64_t;

namespace net
{
	class	Server
	{
		public:
			enum Event
			{
				RECV,
				DISCONNECT,
				CONNECTION,
				NONE,
			};
			int	open(int port, int max_connections = 16)
			{
				struct sockaddr_in	addr;

				_fd = socket(AF_INET, SOCK_STREAM, 0);
				if (_fd == -1)
					return (-1);

				int	yes = 1;

				addr.sin_family = AF_INET;
				addr.sin_port = htons(port);
				addr.sin_addr.s_addr = INADDR_ANY;
				if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
					return (-1);

				if (bind(_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
					return (-1);

				if (listen(_fd, max_connections) == -1)
					return (-1);

				char	buf[INET_ADDRSTRLEN + 1] = {};
				inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf) - 1);

				_addr = buf;
				_port = ntohs(addr.sin_port);

				return (0);
			}
			int	update()
			{
				_pollfds.clear();

				struct pollfd	listen_pfd = {};
				listen_pfd.fd = _fd;
				listen_pfd.events = POLLIN;
				_pollfds.push_back(listen_pfd);

				for (int fd : _clients)
				{
					struct pollfd	pfd = {};
					pfd.fd = fd;
					pfd.events = POLLIN;
					_pollfds.push_back(pfd);
				}

				if (poll(_pollfds.data(), _pollfds.size(), 0) == -1)
					return (-1);

				_poll_index = 0;
				return (0);
			}
			int	recv(void* data, u64 size, Event& event, u64& received_size, int& fd)
			{
				while (_poll_index < _pollfds.size())
				{
					struct pollfd&	pfd = _pollfds[_poll_index++];

					if (!(pfd.revents & (POLLIN | POLLHUP | POLLERR)))
						continue ;

					if (pfd.fd == _fd)
					{
						struct sockaddr_in	client_addr;
						socklen_t			len = sizeof(client_addr);
						int	client_fd = accept(_fd, (struct sockaddr*)&client_addr, &len);

						if (client_fd == -1)
							continue ;

						_clients.push_back(client_fd);
						fd = client_fd;
						event = Event::CONNECTION;
						return (0);
					}

					ssize_t	recv_size = ::recv(pfd.fd, data, size, MSG_DONTWAIT);

					if (recv_size <= 0)
					{
						if (recv_size == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
							continue ;

						fd = pfd.fd;
						disconnect(pfd.fd);
						event = Event::DISCONNECT;
						return (0);
					}

					fd = pfd.fd;
					received_size = recv_size;
					event = Event::RECV;
					return (0);
				}

				event = Event::NONE;
				return (0);
			}
			int	send(int fd, const void* data, u64 size)
			{
				return (::send(fd, data, size, MSG_DONTWAIT));
			}
			int	send_all(const void* data, u64 size)
			{
				for (int fd : _clients)
				{
					if (send(fd, data, size) == -1)
						return (-1);
				}
				return (0);
			}
			void	close()
			{
				for (int fd : _clients)
					::close(fd);
				_clients.clear();
				::close(_fd);
			}

			const std::string&	addr() const {return (_addr);}
			int	port() const {return (_port);}
		private:
			void	disconnect(int fd)
			{
				::close(fd);
				_clients.erase(std::remove(_clients.begin(), _clients.end(), fd), _clients.end());
			}

			int					_port;
			std::string			_addr;

			int					_fd;
			std::vector<int>	_clients;

			std::vector<struct pollfd>	_pollfds;
			size_t						_poll_index = 0;
	};
};

/*
	void	server(int port)
	{
		net::Server	server;

		if (server.open(port) == -1)
			return ;

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
					server.send(fd, buf, size);
			} while (event != net::Server::Event::NONE);
		}
		server.close();
	}
 */
