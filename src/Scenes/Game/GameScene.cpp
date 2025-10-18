/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScene.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:02:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 13:03:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameScene.hpp"
#include "Game.hpp"
#include "Quad.hpp"
#include "Game.hpp"
#include "ImprovedButton.hpp"
#include "TitleScene.hpp"
#include "ErrorScene.hpp"
#include "ImprovedText.hpp"

#include "Perlin2D.hpp"
#include "Spline.hpp"

void	GameScene::_generateMap()
{
	ShaderManager &shaders = _game->getShaders();

	_mapPanel.clear();

	for (int x = 0; x < 200; x++)
	{
		for (int y = 0; y < 200; y++)
		{
			std::string name = "test_quad" + std::to_string(x) + "_" + std::to_string(y);
			float	pixelSize = 1.0;
			float	screenX = (x * pixelSize) - 100;
			float	screenY = (y * pixelSize) - 100;

			float	contNoise = Perlin2D::calcNoise(glm::vec2(x, y), 0.005, 1, 6);
			int height = _generator.getContinentalness(contNoise);

			float	riverNoise = std::abs(Perlin2D::calcNoise(glm::vec2(x, y), 0.003, 1, 4));
			height += _generator.getPeaksValleys(riverNoise);

			glm::vec3	color = glm::vec3(0, 0.9, 0.1) / 1.5f;

			if (height <= 60)
				color = glm::vec3(0, 0, 1) / 1.5f;

			if (height >= 100)
				color = glm::vec3(0.7) / 1.5f;

			if (height >= 150)
				color = glm::vec3(1.0) / 1.5f;

			if (height > 75 && height < 100)
				color = glm::vec3(0, 0.8, 0.2) / 1.5f;

			_mapPanel.add(name, new Quad(glm::vec4(color, 1), glm::vec2(pixelSize, pixelSize), glm::vec2(0.5), glm::vec2(screenX, screenY), shaders.get("quad")));
		}
	}
}

void	GameScene::onEnter()
{
	if (_loaded)
		return ;

	ShaderManager &shaders = _game->getShaders();
	TextureManager &textures = _game->getTextures();

	_generator.load();

	_generateMap();

	UIElement *tmp = _panel.add("back", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(0, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new ImprovedText("Back", 1, glm::vec2(0.5, 1.0), glm::vec2(0, -14), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	_panel.add("fps", new ImprovedText("0 fps", 1, glm::vec2(0), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	tmp = _panel.add("reload", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(0, -28), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			extern uint seed;
			seed = rand();
			this->_generateMap();
		});
	_panel.add("reload_text", new ImprovedText("Reload", 1, glm::vec2(0.5, 1.0), glm::vec2(0, -34), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	_loaded = true;
}

void	GameScene::onExit()
{
}

void	GameScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	GameScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;
	_panel.handleEvents(events);

	static_cast<ImprovedText*>(_panel.get("fps"))->setText(std::to_string((int)(1.0f / _game->getWindow().getDeltaTime())));
}

void	GameScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
	_mapPanel.draw(window.getSize());
}
