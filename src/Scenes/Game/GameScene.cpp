/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScene.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:02:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/06 14:14:21 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameScene.hpp"
#include "Button.hpp"
#include "Image.hpp"
#include "Text.hpp"
#include "Game.hpp"
#include "Quad.hpp"
#include "Game.hpp"
#include "ImprovedButton.hpp"
#include "TitleScene.hpp"
#include "ImprovedText.hpp"

#include "Perlin2D.hpp"

struct	SplinePoint
{
	float	x;
	float	y;
};

struct	Spline
{
	std::vector<SplinePoint>	points;
};

float	getValueInSpline(const Spline &spline, float value)
{
	if (value <= spline.points.front().x)
		return (spline.points.front().y);
	if (value >= spline.points.back().x)
		return (spline.points.back().y);

	for (size_t i = 0; i < spline.points.size() - 1; i++)
	{
		const SplinePoint& p1 = spline.points[i];
		const SplinePoint& p2 = spline.points[i + 1];

		if (value >= p1.x && value <= p2.x)
		{
			float t = (value - p1.x) / (p2.x - p1.x);
			return (glm::mix(p1.y, p2.y, t));
		}
	}
	return (0.0f);
}

Spline continentalnessToHeight =
{
	{
		{ -1.0f, 20},  // deep ocean
		{ -0.3f, 62},  // shallow ocean
		{ -0.15f,  64},  // plains
		{ 0.1f,  72},  // plains
		{ 0.5f,  120},  // hills
		{ 1.0f,  255.0f}  // mountains
	}
};

Spline peaksValleysToHeight =
{
	{
		{0, -15},
		{0.08, 0},
		{0.5, 0},
		{0.6, 32},
		{1, 64}
	}
};

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
			int height = getValueInSpline(continentalnessToHeight, contNoise);

			float	riverNoise = std::abs(Perlin2D::calcNoise(glm::vec2(x, y), 0.003, 1, 4));
			height += getValueInSpline(peaksValleysToHeight, riverNoise);

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

	_generateMap();

	UIElement *tmp = _panel.add("back", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(0, -8), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new Text("Back", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -14), glm::vec2(0.5, 1.0)));
	
	_panel.add("fps", new Text("0 fps", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 0), glm::vec2(0)));

	tmp = _panel.add("reload", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5, 1), glm::vec2(0, -28), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			extern uint seed;
			seed = rand();
			this->_generateMap();
		});
	_panel.add("reload_text", new Text("Reload", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -34), glm::vec2(0.5, 1.0)));


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

	static_cast<Text*>(_panel.get("fps"))->setText(std::to_string((int)(1.0f / _game->getWindow().getDeltaTime())));
}

void	GameScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
	_mapPanel.draw(window.getSize());
}
