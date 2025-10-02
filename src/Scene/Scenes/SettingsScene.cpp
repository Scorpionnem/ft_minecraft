/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SettingsScene.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 10:38:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SettingsScene.hpp"
#include "TitleScene.hpp"
#include "Button.hpp"
#include "Game.hpp"

void	SettingsScene::onEnter()
{
	_loaded = true;
}

void	SettingsScene::onExit()
{
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

}

void	SettingsScene::render()
{
}
