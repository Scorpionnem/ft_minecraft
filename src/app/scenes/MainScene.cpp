#include "app/scenes/MainScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"

void MainScene::init(Client&)
{
	mbl::loader::texture::stb::load("assets/textures/ui/title/ft_minecraft.png", _title_texture);
	_title_texture.upload();
}

SceneCommand MainScene::update(Client& client, const mbl::platform::Input& input)
{
	if (input.close() || input.isDown(SDLK_ESCAPE))
		return { .action = SceneAction::QUIT };

	mbl::ui::sprite(&client.background_tex(), 0, vec2i(client.window().width(), client.window().height()), ANCHOR_TOP_LEFT, true);

	if (_show_options)
	{
	    _show_options = update_options(client.opts());
		return {};
	}

	mbl::ui::sprite(&_title_texture, vec2i(0, -84), vec2i(256, 128), ANCHOR_CENTER);

	if (mbl::ui::button("Singleplayer", vec2f(0, -24.0), vec2f(204, 20), ANCHOR_CENTER))
	{
		client.singleplayer() = true;
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::GAME};
		// return {.action = SceneAction::SWITCH, .targetScene = SceneTag::SINGLEPLAYER};
	}
	if (mbl::ui::button("Multiplayer", vec2f(0, 0.0), vec2f(204, 20), ANCHOR_CENTER))
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MULTIPLAYER};

	if (mbl::ui::button("Options", vec2f(-52, 24.0), vec2f(100, 20), ANCHOR_CENTER))
	    _show_options = true;
	if (mbl::ui::button("Quit game", vec2f(52, 24.0), vec2f(100, 20), ANCHOR_CENTER))
		return { .action = SceneAction::QUIT };
	return {};
}

void MainScene::render(Client&) {}

void MainScene::unload(Client&) {}
