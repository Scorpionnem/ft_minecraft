/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 11:51:52 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TitleScene.hpp"
#include "Game.hpp"
#include "OptionsScene.hpp"
#include "MultiplayerScene.hpp"
#include "SingleplayerScene.hpp"
#include "GameScene.hpp"
#include "Button.hpp"
#include "Checkbox.hpp"
#include "Image.hpp"
#include "BackgroundImage.hpp"
#include "Text.hpp"
#include "Toggle.hpp"
#include "TextField.hpp"

void	TitleScene::onEnter()
{
	_loaded = true;

	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	_panel.add("background", new BackgroundImage(textures.get("dirt"), shaders.get("background")));

	UIElement *tmp = _panel.add("options", new Button(textures.get("button_small"), textures.get("button_highlighted_small"), shaders.get("image"),
													glm::vec2(-50, 40), glm::vec2(0.5, 0.5), glm::vec2(1, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new OptionsScene(_game));
		});
	_panel.add("options_text", new Text("Options", textures.get("ascii"), shaders.get("font"), glm::vec2(-50, 40), glm::vec2(0.5)));

	tmp = _panel.add("quit", new Button(textures.get("button_small"), textures.get("button_highlighted_small"), shaders.get("image"),
										glm::vec2(50, 40), glm::vec2(0.5, 0.5), glm::vec2(1, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			_game->setRunning(false);
		});
	_panel.add("quit_text", new Text("Quit game", textures.get("ascii"), shaders.get("font"), glm::vec2(50, 40), glm::vec2(0.5)));

	tmp = _panel.add("singpleplayer", new Button(textures.get("button"), textures.get("button_highlighted"), shaders.get("image"),
											glm::vec2(0, -20), glm::vec2(0.5, 0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new SingleplayerScene(_game));
		});
	_panel.add("singleplayer_text", new Text("Singleplayer", textures.get("ascii"), shaders.get("font"), glm::vec2(0, -20), glm::vec2(0.5)));

	tmp = _panel.add("multiplayer", new Button(textures.get("button"), textures.get("button_highlighted"), shaders.get("image"),
									glm::vec2(0, 0), glm::vec2(0.5, 0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new MultiplayerScene(_game));
		});
	_panel.add("multiplayer_text", new Text("Multiplayer", textures.get("ascii"), shaders.get("font"), glm::vec2(0, 0), glm::vec2(0.5)));

	_panel.add("icon", new Image(textures.get("ft_minecraft"), shaders.get("image"),
							glm::vec2(0, -90), glm::vec2(0.5, 0.5), glm::vec2(0.3, 0.3)));

	_panel.add("credits", new Text("By mbatty & mbirou", textures.get("ascii"), shaders.get("font"), glm::vec2(0, 0), glm::vec2(1)));

	tmp = _panel.add("splash_text", new Text("Also try ft_vox!", textures.get("ascii"), shaders.get("font"), glm::vec2(115, -85), glm::vec2(0.5)));
	static_cast<Text*>(tmp)->setAngle(-15);
	static_cast<Text*>(tmp)->setColor(glm::vec3(1.0, 1.0, 0.0));
}

void	TitleScene::onExit()
{
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

	_panel.handleEvents(events);

	static_cast<Text*>(_panel.get("splash_text"))->setScale(glm::vec2(1.0 + abs(cos(glfwGetTime() * 4) / 6)));
}

void	TitleScene::render()
{
	Window	&window = _game->getWindow();

	glDisable(GL_DEPTH_TEST);

	_panel.draw(window.getSize());
}
