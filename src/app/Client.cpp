#include "app/Client.hpp"

#include "stdbool.h"

void    Client::init()
{
	_win.open("ft_minecraft", 860, 520);

	mbl::ui::init("assets/textures/font/ascii.png");

	mbl::loader::texture::stb::load("assets/textures/ui/dirt_background.png", _background_tex);
	_background_tex.upload();

	_running = true;

	_sceneManager.switchScene(*this, SceneTag::MAIN);
}

void    Client::loop()
{
	while (_running)
	{
		const mbl::platform::Input&	input = _win.pollEvents();
		mbl::ui::beginFrame(input);

		if (input.resize())
            glViewport(0, 0, _win.width(), _win.height());

		if (!_sceneManager.update(*this, input))
		{
			_running = false;
			continue;
		}

		_sceneManager.render(*this);

		mbl::ui::render();
		_win.swapBuffers();
	}
}
