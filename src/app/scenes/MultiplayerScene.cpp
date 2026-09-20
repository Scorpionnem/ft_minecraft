#include "app/scenes/MultiplayerScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "net/LAN.hpp"
#include "app/Client.hpp"

void MultiplayerScene::init(Client&)
{
	if (_broadcast.connect("224.0.0.42", 6767) == -1)
		throw std::runtime_error(std::string(strerror(errno)));
}

SceneCommand MultiplayerScene::update(Client& client, const mbl::platform::Input& input)
{
	if (input.close() || input.isDown(SDLK_ESCAPE))
		return { .action = SceneAction::QUIT };

	mbl::ui::text("Play Multiplayer", vec2f(0, 4), vec2f(0.5, 0.0));

	_update_broadcast();
	_remove_outdated_servers();
	SceneCommand cmd = _list_servers(client);
	if (cmd.action != SceneAction::NONE)
	{
		client.addr() = _addr;
		client.port() = _port;
		client.singleplayer() = false;
		return (cmd);
	}

	if (mbl::ui::button("Cancel", vec2f(0, -4.0), vec2f(100, 20), vec2f(0.5, 1.0)))
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MAIN};
	return {};
}

SceneCommand	MultiplayerScene::_list_servers(Client& client)
{
	float	offset = 0.0 - ((float)_servers.size() - 1.0) / 2.0;
	for (auto& serv : _servers)
	{
		std::string	btn_str = std::string(serv.addr) + ":" + std::to_string(serv.port);
		vec2i	btn_size = vec2i(96, 16);

		if (mbl::ui::button(btn_str, vec2f(0, offset * btn_size.y()), btn_size, vec2f(0.5, 0.5)))
		{
			_addr = serv.addr;
			_port = serv.port;
			return { .action = SceneAction::SWITCH, .targetScene = SceneTag::GAME };
		}
		offset += 1;
	}
	return {};
}

void	MultiplayerScene::_remove_outdated_servers()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);

	#define SERVER_BROADCAST_DELAY 1
	auto i = _servers.begin();
	while (i != _servers.end())
	{
		LANServer&	serv = *i;

	    if (tv.tv_sec - serv.last_updt_ts > SERVER_BROADCAST_DELAY)
	       	i = _servers.erase(i);
	    else
	        ++i;
	}
}

void	MultiplayerScene::_update_broadcast()
{
	mbl::net::MulticastReceiver::Event	event;
	u8					buf[4096];
	u64					size;

	std::vector<std::vector<LANServer>::iterator>	deletes;

	do
	{
		if (_broadcast.recv(buf, sizeof(buf), event, size) == -1)
		{
			throw std::runtime_error(std::string(strerror(errno)));
		}
		if (event == mbl::net::MulticastReceiver::Event::RECV)
		{
			_dispatch_packet(buf, size);
		}
	} while (event != mbl::net::MulticastReceiver::Event::NONE);
}

void	MultiplayerScene::_dispatch_packet(u8 *data, u64 size)
{
	if (size < sizeof(Packet::Header))
		return ;

	Packet::Header*	hdr = reinterpret_cast<Packet::Header*>(data);
	if (hdr->magic != MINECRAFT_PCKT_MAGIC)
		return ;

	switch (hdr->type)
	{
		case LANBROADCAST_TYPE:
		{
			Packet::LANBroadcast*	lan_pckt = reinterpret_cast<Packet::LANBroadcast*>(data);

			struct timeval tv;
			gettimeofday(&tv,NULL);

			LANServer	serv = LANServer{.addr = lan_pckt->addr, .port = lan_pckt->port, .last_updt_ts = tv.tv_sec};

			auto find = std::find_if(_servers.begin(), _servers.end(), [&serv](const LANServer& b){return (serv.addr == b.addr && serv.port == b.port);});
			if (find == _servers.end())
				_servers.push_back(serv);
			else
				find->last_updt_ts = tv.tv_sec;

			break ;
		}
		default :
			return ;
	}
}

void MultiplayerScene::render(Client&) {}

void MultiplayerScene::unload(Client&)
{
	_broadcast.disconnect();
}
