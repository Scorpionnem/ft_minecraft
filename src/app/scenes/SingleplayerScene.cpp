#include "app/scenes/SingleplayerScene.hpp"
#include "app/scene/SceneManager.hpp"

void SingleplayerScene::init() {}

SceneCommand SingleplayerScene::update(Client &, const mbl::platform::Input& input)
{
	if (input.close() || input.isDown(SDLK_ESCAPE))
		return { .action = SceneAction::QUIT };

	mbl::ui::text("Play Singleplayer", vec2f(0, 4), vec2f(0.5, 0.0));

	if (mbl::ui::button("Cancel", vec2f(0, -4.0), vec2f(100, 20), vec2f(0.5, 1.0)))
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MAIN};
	return {};
}

void SingleplayerScene::render() {}

void SingleplayerScene::unload() {}
