#include "app/scene/SceneManager.hpp"
#include "app/scenes/MainScene.hpp"
#include "app/scenes/MultiplayerScene.hpp"
#include "app/scenes/SingleplayerScene.hpp"
#include "app/scenes/GameScene.hpp"

SceneManager::SceneManager()
{
	_scenes[SceneTag::MAIN] = std::make_shared<MainScene>();
	_scenes[SceneTag::MULTIPLAYER] = std::make_shared<MultiplayerScene>();
	_scenes[SceneTag::SINGLEPLAYER] = std::make_shared<SingleplayerScene>();
	_scenes[SceneTag::GAME] = std::make_shared<GameScene>();
}

SceneManager::~SceneManager()
{
}

bool SceneManager::update(Client& client, const mbl::platform::Input & input)
{
	if (_current == SceneTag::NONE)
		return true;
	const SceneCommand & sceneCommand = _scenes[_current]->update(client, input);
	if (sceneCommand.action == SceneAction::SWITCH)
	{
		if (sceneCommand.targetScene == SceneTag::NONE)
			throw std::runtime_error("TargetScene is set to NONE for a SWITCH action.");
		switchScene(client, sceneCommand.targetScene);
	}
	else if (sceneCommand.action == SceneAction::QUIT)
		return false;
	return true;
}

void SceneManager::render(Client& client)
{
	if (_current != SceneTag::NONE)
		_scenes[_current]->render(client);
}

void SceneManager::switchScene(Client& client, SceneTag nextScene)
{
	if (_scenes.find(nextScene) == _scenes.end())
	{
		throw std::runtime_error("Scene with enum index #" + std::to_string(static_cast<int>(nextScene))
			+ " was never created in SceneManager's constructor.");
	}
	if (_current != SceneTag::NONE)
		_scenes[_current]->unload(client);
	_scenes[nextScene]->init(client);
	_current = nextScene;
}

void SceneManager::unload(Client& client)
{
	if (_current != SceneTag::NONE)
		_scenes[_current]->unload(client);
}
