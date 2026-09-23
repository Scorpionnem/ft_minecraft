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
		void	_show_f3(Client& client, const mbl::platform::Input& input);
		void	_update_net(Client& client);
		void    _updateCamera(const mbl::platform::Input& input);

		void	_dispatch_packet(Client& client, u8 *data, u64 size);

		mbl::render::Shader         _mesh_shader;
    	mbl::render::Mesh           _mesh;
    	mbl::render::TextureAtlas   _atlas;

		std::thread				_serverThread;
		std::shared_ptr<Server>	_server;

		mbl::render::Camera		_cam;

		mbl::utils::Chrono	_server_updt_time;

		bool					_paused = false;

		mbl::net::Client		_netClient;
};
