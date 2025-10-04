/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionsScene.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/04 18:35:50 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OptionsScene.hpp"
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

void	OptionsScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	if (_loaded)
		return ;

	_loaded = true;

	_panel.add("background", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5));

	UIElement *tmp = _panel.add("done", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
										glm::vec2(0, -8), glm::vec2(0.5, 1), glm::vec2(1, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new TitleScene(_game));
		});
	_panel.add("done_text", new Text("Done", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -14), glm::vec2(0.5, 1.0)));

	_panel.add("options_text", new Text("Options", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 14), glm::vec2(0.5, 0)));
}

void	OptionsScene::onExit()
{
}

void	OptionsScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	OptionsScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;
	
	_panel.handleEvents(events);
}

void	OptionsScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
}
