/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 15:00:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TitleScene.hpp"
#include "Button.hpp"
#include "Image.hpp"
#include "Game.hpp"

void	TitleScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	
	_settingsButton = new Button(textures.get("button"), textures.get("button_highlighted"), glm::vec2(-100, 80), glm::vec2(0.5, 0.5), glm::vec2(1, 2));
	static_cast<Button*>(_settingsButton)->setClickFunc(
		[this]()
		{
			this->requestScene("settings");
		});

	_quitButton = new Button(textures.get("button"), textures.get("button_highlighted"), glm::vec2(100, 80), glm::vec2(0.5, 0.5), glm::vec2(1, 2));
	static_cast<Button*>(_quitButton)->setClickFunc(
		[this]()
		{
			_game->setRunning(false);
		});

	_soloButton = new Button(textures.get("button"), textures.get("button_highlighted"), glm::vec2(0, 0), glm::vec2(0.5, 0.5), glm::vec2(2, 2));
	_multiplayerButton = new Button(textures.get("button"), textures.get("button_highlighted"), glm::vec2(0, -40), glm::vec2(0.5, 0.5), glm::vec2(2, 2));
	_icon = new Image(textures.get("ft_minecraft"), glm::vec2(0, -160), glm::vec2(0.5, 0.5), glm::vec2(0.5, 0.5));
}

void	TitleScene::onExit()
{
	delete _settingsButton;
}

void	TitleScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	TitleScene::update(float deltaTime)
{
	(void)deltaTime;
	UIEvent	events;

	events.mousePos = _game->getWindow().getMousePos();
	events.windowSize = _game->getWindow().getSize();
	events.inputs = &_game->getInput();

	_settingsButton->handleEvents(events);
	_quitButton->handleEvents(events);
	_soloButton->handleEvents(events);
	_multiplayerButton->handleEvents(events);
}

void	TitleScene::render()
{
	ShaderManager &shaders = _game->getShaders();
	Window	&window = _game->getWindow();

	shaders.get("image")->setMat4("projection", glm::ortho(0.f, window.getWidth(), window.getHeight(), 0.f, -1.f, 1.f));
	_settingsButton->draw(shaders.get("image"), window.getSize());
	_quitButton->draw(shaders.get("image"), window.getSize());
	_soloButton->draw(shaders.get("image"), window.getSize());
	_multiplayerButton->draw(shaders.get("image"), window.getSize());
	_icon->draw(shaders.get("image"), window.getSize());
}
