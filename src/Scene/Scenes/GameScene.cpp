/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScene.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:02:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 12:55:07 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GameScene.hpp"
#include "Button.hpp"
#include "Image.hpp"
#include "Text.hpp"
#include "Game.hpp"

void	GameScene::onEnter()
{
	if (_loaded)
		return ;

	_loaded = true;
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
}
