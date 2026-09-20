#pragma once

#include <unordered_map>
#include <memory>
#include "app/scene/Scene.hpp"

enum class SceneTag
{
	MAIN,
	MULTIPLAYER,
	SINGLEPLAYER,
	GAME,
	NONE,
};

enum class SceneAction
{
	NONE,
	SWITCH,
	QUIT
};

struct SceneCommand
{
	SceneAction	action = SceneAction::NONE;
	SceneTag		targetScene = SceneTag::NONE;
};

class SceneManager
{
	public:
		SceneManager();
		~SceneManager();

		bool			update(Client& client, const mbl::platform::Input& input);
		void			render();
		void			switchScene(SceneTag nextScene);
		void			unload();

	private:
		std::unordered_map<SceneTag, std::shared_ptr<Scene>>	_scenes;
		SceneTag		_current = SceneTag::NONE;
};
