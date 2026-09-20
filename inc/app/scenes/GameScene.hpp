#pragma once

#include "app/scene/Scene.hpp"
#include "app/Server.hpp"

class GameScene: public Scene
{
	public:
		GameScene() = default;
		~GameScene() = default;

		void			init(Client& client) override;
		SceneCommand	update(Client & client, const mbl::platform::Input& input) override;
		void			render(Client& client) override;
		void			unload(Client& client) override;
	private:
		void	_update_net();
		std::thread				_serverThread;
		std::shared_ptr<Server>	_server;

		mbl::net::Client		_netClient;
};
