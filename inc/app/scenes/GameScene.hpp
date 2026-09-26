#pragma once

#include "app/scene/Scene.hpp"
#include "app/Server.hpp"
#include "game/world/World.hpp"
#include "game/entity/Entities.hpp"

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

		int netSend(const void *data, u64 size)
		{
			_tx_pckt++;
			return (_netClient.send(data, size));
		}

		void	_dispatch_packet(Client& client, u8 *data, u64 size);

		static constexpr u16	RENDER_DISTANCE = 8; // in chunks
		static constexpr u32	MAX_PACKETS_PER_FRAME = 512;

		mbl::render::Shader         _mesh_shader;
    	mbl::render::Mesh           _mesh;
    	mbl::render::TextureAtlas   _atlas;

		std::thread				_serverThread;
		std::shared_ptr<Server>	_server;

		mbl::render::Camera		_fp_cam; // first person
		mbl::render::Camera		_tp_cam; // third person
		mbl::render::Camera		_transition_cam; // third person
		mbl::render::Camera*	_render_cam = nullptr; // third person
		bool	_tp_toggle = false;
		float	_tp_distance_target_set = 3;
		float	_tp_distance = 3;
		bool	_moving = false;

		int	_fps = 0;
		int	_rx_pckt = 0;
		int	_tx_pckt = 0;

		mbl::utils::Chrono	_server_updt_time;

		bool					_paused = false;

		mbl::net::Client		_netClient;

		mbl::utils::ThreadPool	_chunkThreads;
		World				_world;
		Entities			_entities;
	private:
};
