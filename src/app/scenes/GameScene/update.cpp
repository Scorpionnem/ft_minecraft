#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"

SceneCommand GameScene::update(Client& client, const mbl::platform::Input& input)
{
	try
	{
		if (input.close() || (input.wasPressed(SDLK_ESCAPE) && input.isDown(SDLK_LCTRL)))
			return { .action = SceneAction::QUIT };

		if (input.wasPressed(SDLK_ESCAPE))
			_toggle_pause(client);
		if (input.wasPressed(SDLK_F3))
			_debug = !_debug;

		if (input.resize() && !_paused)
			_render_buffer.resize(client.window().width(), client.window().height());

		if (_paused && !_show_options)
		{
			if (mbl::ui::button("Back to Game", vec2f(0, -24), vec2f(200, 20), ANCHOR_CENTER))
				_toggle_pause(client);
			if (mbl::ui::button("Options", 0, vec2f(200, 20), ANCHOR_CENTER))
				_show_options = true;
			if (mbl::ui::button("Save and Quit to Title", vec2f(0, 24), vec2f(200, 20), ANCHOR_CENTER))
				return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MAIN};
		}
		if (_show_options)
		{
			_options_screen(client, input);
		}

		if (_tick_timer.get() >= (1.0 / 20.0))
		{
			_tick(client, input);
			_tick_timer.start();
		}

		_updateCamera(input);

		_update_net();
	} catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MULTIPLAYER};
	}
	return {};
}

void	GameScene::_toggle_pause(Client& client)
{
	_paused = !_paused;
	if (_paused)
		_render_buffer.resize(430, 260);
	else
		_render_buffer.resize(client.window().width(), client.window().height());
	_show_options = false;
	client.window().captureMouse(!_paused);
}

void	GameScene::_tick(Client& client, const mbl::platform::Input& input)
{
	(void)client;

	_fps = 1.0 / input.delta();
	_rx_pckt = 0; _tx_pckt = 0;

	Packet::PlayerPos	ppos_pckt = {};

	ppos_pckt.x = _fp_cam.pos.x();
	ppos_pckt.y = _fp_cam.pos.y();
	ppos_pckt.z = _fp_cam.pos.z();
	_netClient.send(&ppos_pckt, sizeof(ppos_pckt));

	_world.update();
	_world.clearUnused({worldToChunkWorld(_fp_cam.pos, Chunk::SIZE)}, RENDER_DISTANCE);
	_world.requestInRange(worldToChunkWorld(_fp_cam.pos, Chunk::SIZE), RENDER_DISTANCE, _netClient, _tx_pckt);
}
