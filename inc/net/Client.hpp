#pragma once

#include <cerrno>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>
#include <stdbool.h>
#include <cstdint>

using u8 = uint8_t;
using u64 = uint64_t;

namespace net
{
	class	Client
	{
		public:
			enum Event
			{
				RECV,
				DISCONNECT,
				NONE,
			};
		public:
			Client() {}
			~Client()
			{
				disconnect();
			}
			int connect(const char* address, int port)
			{
				struct sockaddr_in	serv_addr;

				_fd = socket(AF_INET, SOCK_STREAM, 0);
				if (_fd == -1)
					return (-1);

				serv_addr.sin_family = AF_INET;
				serv_addr.sin_port = htons(port);
				if (inet_pton(AF_INET, address, &serv_addr.sin_addr) == -1)
					return (-1);

				if (::connect(_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
					return (-1);
				return (0);
			}
			void	disconnect()
			{
				if (_fd != -1)
				{
					close(_fd);
					_fd = -1;
				}
			}
			int	recv(void* data, u64 size, Event& event, u64& received_size)
			{
				if (_fd == -1)
				{
					errno = ENOTCONN;
					return (-1);
				}

				ssize_t	recv_size = ::recv(_fd, data, size, MSG_DONTWAIT);
				if (recv_size == -1)
				{
					if (errno == EAGAIN || errno == EWOULDBLOCK)
					{
						event = Event::NONE;
						return (0);
					}

					return (-1);
				}
				if (recv_size == 0)
				{
					disconnect();
					event = Event::DISCONNECT;
					return (0);
				}
				received_size = recv_size;
				event = Event::RECV;
				return (0);
			}
			int	send(const void* data, u64 size)
			{
				return (::send(_fd, data, size, MSG_WAITALL));
			}
		private:
			int	_fd = -1;
	};
};

/*
	void	client(const char* addr, int port)
	{
		net::Client	client;

		if (client.connect(addr, port) == -1)
			return ;

		bool	running = true;
		while (running)
		{
			net::Client::Event	event;
			u8					buf[4096];
			u64					size;

			do
			{
				if (client.recv(buf, sizeof(buf), event, size) == -1)
				{
					running = false;
					break ;
				}
				if (event == net::Client::Event::DISCONNECT)
				{
					running = false;
					break ;
				}

				std::cout << '\r' << "waiting..." << std::flush;
			} while (event != net::Client::Event::NONE);
		}
		client.disconnect();
	}
 */
