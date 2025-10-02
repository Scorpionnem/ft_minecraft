/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplayerScene.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:45:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 12:14:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MultiplayerScene.hpp"
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

void	MultiplayerScene::onEnter()
{
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();
	
	_loaded = true;

	_panel.add("background", new BackgroundImage(textures.get("dirt"), shaders.get("background")));

	UIElement *tmp = _panel.add("back", new Button(textures.get("button"), textures.get("button_highlighted"), shaders.get("image"),
										glm::vec2(0, -8), glm::vec2(0.5, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new TitleScene(_game));
		});
	_panel.add("back_text", new Text("Back", textures.get("ascii"), shaders.get("font"), glm::vec2(0, -14), glm::vec2(0.5, 1.0)));

	_panel.add("ip_text_field", new TextField(textures.get("text_field"), textures.get("text_field_highlighted"), textures.get("ascii"), shaders.get("image"), shaders.get("font"),
									glm::vec2(0), glm::vec2(0.5), 24));
	_panel.add("ip_text_field_text", new Text("Select IP", textures.get("ascii"), shaders.get("font"), glm::vec2(0, -20), glm::vec2(0.5)));

	_panel.add("connect", new Button(textures.get("button_small"), textures.get("button_highlighted_small"), shaders.get("image"),
										glm::vec2(0, 24), glm::vec2(0.5)));
	_panel.add("connect_text", new Text("Connect", textures.get("ascii"), shaders.get("font"), glm::vec2(0, 24), glm::vec2(0.5)));

	_panel.add("multiplayer_text", new Text("Multiplayer", textures.get("ascii"), shaders.get("font"), glm::vec2(0, 14), glm::vec2(0.5, 0)));
}

void	MultiplayerScene::onExit()
{
}

void	MultiplayerScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	MultiplayerScene::update(float deltaTime)
{
	(void)deltaTime;
	UIEvent	events;

	events.mousePos = _game->getWindow().getMousePos();
	events.windowSize = _game->getWindow().getSize();
	events.inputs = &_game->getInput();

	_panel.handleEvents(events);
}

void	MultiplayerScene::render()
{
	Window	&window = _game->getWindow();

	_panel.draw(window.getSize());
}
