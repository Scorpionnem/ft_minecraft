/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleplayerScene.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:45:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 12:55:00 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SingleplayerScene.hpp"
#include "TitleScene.hpp"
#include "Button.hpp"
#include "Game.hpp"
#include "Button.hpp"
#include "Checkbox.hpp"
#include "Image.hpp"
#include "BackgroundImage.hpp"
#include "Text.hpp"
#include "Toggle.hpp"
#include "TextField.hpp"

void	SingleplayerScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();
	
	if (_loaded)
		return ;

	_loaded = true;

	_panel.add("background", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background")));

	UIElement *tmp = _panel.add("back", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), shaders.get("image"),
										glm::vec2(0, -8), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new Text("Back", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -14), glm::vec2(0.5, 1.0)));

	tmp = _panel.add("cookie_button", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), shaders.get("image"),
										glm::vec2(0, 0), glm::vec2(0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->_timesClicked++;
		});
	_panel.add("cookie_button_text", new Text("Click me!", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 0), glm::vec2(0.5)));
	_panel.add("cookie_count_text", new Text("0", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -20), glm::vec2(0.5)));

	_panel.add("singleplayer_text", new Text("Singleplayer", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 14), glm::vec2(0.5, 0)));
}

void	SingleplayerScene::onExit()
{
}

void	SingleplayerScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	SingleplayerScene::update(float deltaTime)
{
	(void)deltaTime;
	UIEvent	events;

	events.mousePos = _game->getWindow().getMousePos();
	events.windowSize = _game->getWindow().getSize();
	events.inputs = &_game->getInput();

	static_cast<Text*>(_panel.get("cookie_count_text"))->setText(std::to_string(_timesClicked));
	
	_panel.handleEvents(events);
}

void	SingleplayerScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
}
