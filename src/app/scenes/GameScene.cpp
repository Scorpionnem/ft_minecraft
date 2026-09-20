#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"

void GameScene::init(Client& client)
{
	if (client.singleplayer())
	{
		_server = std::make_shared<Server>();
		_serverThread = std::thread(
			[this](){_server->run();}
		);

		while (!_server->running())
		{
			usleep(500);
		}

		_netClient.connect(_server->netServer().addr().c_str(), _server->netServer().port());
	}
	else
	{
		_netClient.connect(client.addr().c_str(), client.port());
	}
}

SceneCommand GameScene::update(Client &, const mbl::platform::Input& input)
{
	if (input.close() || input.isDown(SDLK_ESCAPE))
		return { .action = SceneAction::QUIT };

	_update_net();

	mbl::ui::text(std::to_string(_netClient.rtt()), 0, vec2f(0.5, 0.0));

	if (mbl::ui::button("Quit", 0, vec2f(50, 20), ANCHOR_CENTER))
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MAIN};
	return {};
}

void	GameScene::_update_net()
{
	mbl::net::Client::Event	event;
	u8					buf[4096] = {};
	u64					size;

	_netClient.update();
	do
	{
		if (_netClient.recv(buf, sizeof(buf), event, size) == -1)
		{
			perror("");
			// client.last_error() = std::string(strerror(errno));
			// return { .action = SceneAction::SWITCH, .targetScene = Scene::ERROR };
		}
		if (event == mbl::net::Client::Event::DISCONNECT)
		{
			_netClient.disconnect();
		}
		if (event == mbl::net::Client::Event::RECV)
		{
			// _dispatch_packet(buf, size);
		}
	} while (event != mbl::net::Client::Event::NONE);
}

void GameScene::render(Client&)
{

}

void GameScene::unload(Client&)
{
	_netClient.disconnect();

	if (_server)
	{
		_server->stop();
		_serverThread.join();
		_server = nullptr;
	}
}
