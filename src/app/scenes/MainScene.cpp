#include "app/scenes/MainScene.hpp"
#include "app/scene/SceneManager.hpp"

void MainScene::init(Client&) {}

SceneCommand MainScene::update(Client &, const mbl::platform::Input& input)
{
	if (input.close() || input.isDown(SDLK_ESCAPE))
		return { .action = SceneAction::QUIT };

	if (mbl::ui::button("Singleplayer", vec2f(0, -24.0), vec2f(204, 20), ANCHOR_CENTER))
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::SINGLEPLAYER};
	if (mbl::ui::button("Multiplayer", vec2f(0, 0.0), vec2f(204, 20), ANCHOR_CENTER))
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MULTIPLAYER};

	mbl::ui::button("Options", vec2f(-52, 24.0), vec2f(100, 20), ANCHOR_CENTER);
	if (mbl::ui::button("Quit game", vec2f(52, 24.0), vec2f(100, 20), ANCHOR_CENTER))
		return { .action = SceneAction::QUIT };
	return {};
}

void MainScene::render(Client&) {}

void MainScene::unload(Client&) {}
