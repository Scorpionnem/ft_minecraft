/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScene.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:02:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 17:03:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameScene.hpp"
#include "Button.hpp"
#include "Image.hpp"
#include "Text.hpp"
#include "Game.hpp"

void	GameScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	
	_hotbar = new Image(textures.get("hotbar"), glm::vec2(0, 0), glm::vec2(0.5, 1), glm::vec2(1, 1));
	_hotbarSlot = new Image(textures.get("hotbar_slot"), glm::vec2(-60, 0), glm::vec2(0.5, 1), glm::vec2(1, 1));
}

void	GameScene::onExit()
{
}

void	GameScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	GameScene::update(float deltaTime)
{
	(void)deltaTime;
	UIEvent	events;

	events.mousePos = _game->getWindow().getMousePos();
	events.windowSize = _game->getWindow().getSize();
	events.inputs = &_game->getInput();
}

void	GameScene::render()
{
	ShaderManager &shaders = _game->getShaders();
	Window	&window = _game->getWindow();

	glDisable(GL_DEPTH_TEST);

	shaders.get("image")->setMat4("projection", glm::ortho(0.f, window.getWidth(), window.getHeight(), 0.f, -1.f, 1.f));
	shaders.get("font")->setMat4("projection", glm::ortho(0.f, window.getWidth(), window.getHeight(), 0.f, -1.f, 1.f));
	
	_hotbar->draw(shaders.get("image"), window.getSize());
	_hotbarSlot->draw(shaders.get("image"), window.getSize());
}
