#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"

void GameScene::unload(Client& client)
{
	client.window().captureMouse(false);

	_netClient.disconnect();

	if (_server)
	{
		_server->stop();
		_serverThread.join();
		_server = nullptr;
	}
}
