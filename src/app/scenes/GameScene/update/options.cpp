#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"

void	GameScene::_options_screen(Client& client, const mbl::platform::Input& input)
{
	mbl::ui::text("Options", vec2f(0, 4), vec2f(0.5, 0.0));

	if (mbl::ui::button("Done", vec2f(0, -4.0), vec2f(100, 20), vec2f(0.5, 1.0)))
	{
		_show_options = false;
		return ;
	}

	(void)client;(void)input;
}
