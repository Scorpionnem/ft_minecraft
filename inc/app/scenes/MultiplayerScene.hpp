#pragma once

#include "app/scene/Scene.hpp"
#include "net/LAN.hpp"

class MultiplayerScene: public Scene
{
	public:
		MultiplayerScene() = default;
		~MultiplayerScene() = default;

		void			init(Client& client) override;
		SceneCommand	update(Client & client, const mbl::platform::Input& input) override;
		void			render(Client& client) override;
		void			unload(Client& client) override;
	private:
		void	_update_broadcast();
		void	_dispatch_packet(u8 *data, u64 size);
		void	_remove_outdated_servers();
		SceneCommand	_list_servers(Client& client);

		std::vector<LANServer>		_servers;
		mbl::net::MulticastReceiver	_broadcast;

		std::string	_addr;
		int			_port;
};
