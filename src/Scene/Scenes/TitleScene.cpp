/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 12:03:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TitleScene.hpp"
#include "Button.hpp"
#include "Checkbox.hpp"
#include "Image.hpp"
#include "Text.hpp"
#include "Toggle.hpp"
#include "TextField.hpp"
#include "Game.hpp"
#include "SettingsScene.hpp"
#include "GameScene.hpp"

void	TitleScene::onEnter()
{
	_loaded = true;

	TextureManager &textures = _game->getTextures();

	_settingsButton = new Button(textures.get("button"), textures.get("button_highlighted"), glm::vec2(-50, 40), glm::vec2(0.5, 0.5), glm::vec2(0.5, 1));
	static_cast<Button*>(_settingsButton)->setClickFunc(
		[this]()
		{
			this->requestScene(new SettingsScene(_game));
		});

	_quitButton = new Button(textures.get("button"), textures.get("button_highlighted"), glm::vec2(50, 40), glm::vec2(0.5, 0.5), glm::vec2(0.5, 1));
	static_cast<Button*>(_quitButton)->setClickFunc(
		[this]()
		{
			_game->setRunning(false);
		});

	_soloButton = new Button(textures.get("button"), textures.get("button_highlighted"), glm::vec2(0, -20), glm::vec2(0.5, 0.5));
	static_cast<Button*>(_soloButton)->setClickFunc(
		[this]()
		{
			this->requestScene(new GameScene(_game));
		});

	_multiplayerButton = new TextField(textures.get("text_field"), textures.get("text_field_highlighted"), textures.get("ascii"), glm::vec2(0, 0), glm::vec2(0.5, 0.5), 24);
	static_cast<TextField*>(_multiplayerButton)->setClickFunc(
		[this](const std::string &str)
		{
			_game->getWindow().setWindowName(str);
		});
		
	_icon = new Image(textures.get("ft_minecraft"), glm::vec2(0, -80), glm::vec2(0.5, 0.5), glm::vec2(0.25, 0.25));
	_textTest = new Text("Singleplayer", textures.get("ascii"), glm::vec2(0, -20), glm::vec2(0.5));
	_textTest1 = new Text("Multiplayer", textures.get("ascii"), glm::vec2(0, 0), glm::vec2(0.5));
	_textTest2 = new Text("Options", textures.get("ascii"), glm::vec2(-50, 40), glm::vec2(0.5));
	_textTest3 = new Text("Quit game", textures.get("ascii"), glm::vec2(50, 40), glm::vec2(0.5));
	_textAuthor = new Text("By mbatty & mbirou", textures.get("ascii"), glm::vec2(0, 0), glm::vec2(1));

	_fpsText = new Text("fps", textures.get("ascii"), glm::vec2(0, 0), glm::vec2(0));
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

	static_cast<Text*>(_fpsText)->setText(std::to_string((int)(1.0f / deltaTime)) + " fps");

	_settingsButton->handleEvents(events);
	_quitButton->handleEvents(events);
	_soloButton->handleEvents(events);
	_multiplayerButton->handleEvents(events);
}

void	TitleScene::render()
{
	ShaderManager &shaders = _game->getShaders();
	Window	&window = _game->getWindow();

	glDisable(GL_DEPTH_TEST);

	shaders.get("image")->setMat4("projection", glm::ortho(0.f, window.getWidth(), window.getHeight(), 0.f, -1.f, 1.f));
	shaders.get("font")->setMat4("projection", glm::ortho(0.f, window.getWidth(), window.getHeight(), 0.f, -1.f, 1.f));

	_settingsButton->draw(shaders.get("image"), window.getSize());
	_quitButton->draw(shaders.get("image"), window.getSize());
	_soloButton->draw(shaders.get("image"), window.getSize());
	static_cast<TextField*>(_multiplayerButton)->draw(shaders.get("image"), shaders.get("font"), window.getSize());
	_icon->draw(shaders.get("image"), window.getSize());
	_textTest->draw(shaders.get("font"), window.getSize());
	// _textTest1->draw(shaders.get("font"), window.getSize());
	_textTest2->draw(shaders.get("font"), window.getSize());
	_textTest3->draw(shaders.get("font"), window.getSize());
	_textAuthor->draw(shaders.get("font"), window.getSize());

	_fpsText->draw(shaders.get("font"), window.getSize());
}
