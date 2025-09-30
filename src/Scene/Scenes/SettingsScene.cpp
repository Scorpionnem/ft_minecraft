/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SettingsScene.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 21:47:05 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SettingsScene.hpp"
#include "TitleScene.hpp"
#include "Button.hpp"
#include "Game.hpp"

void	SettingsScene::onEnter()
{
	_loaded = true;

	TextureManager &textures = _game->getTextures();

	_quitButton = new Button(textures.get("button"), textures.get("button_highlighted"), glm::vec2(0, 0), glm::vec2(0.5, 0), glm::vec2(2, 2));
	static_cast<Button*>(_quitButton)->setClickFunc(
		[this]()
		{
			this->requestScene(new TitleScene(_game));
		});
}

void	SettingsScene::onExit()
{
	delete _quitButton;
}

void	SettingsScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	SettingsScene::update(float deltaTime)
{
	(void)deltaTime;
	UIEvent	events;

	events.mousePos = _game->getWindow().getMousePos();
	events.windowSize = _game->getWindow().getSize();
	events.inputs = &_game->getInput();

	_quitButton->handleEvents(events);
}

void	SettingsScene::render()
{
	ShaderManager &shaders = _game->getShaders();
	Window	&window = _game->getWindow();

	shaders.get("image")->setMat4("projection", glm::ortho(0.f, window.getWidth(), window.getHeight(), 0.f, -1.f, 1.f));
	_quitButton->draw(shaders.get("image"), window.getSize());
}
