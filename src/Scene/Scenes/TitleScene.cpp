/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 16:51:14 by mbatty           ###   ########.fr       */
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

constexpr uint SPLASH_TEXT_COUNT = 26;
constexpr double SPLASH_TEXT_INTERVAL = 10.0;

/*
	Thanks #world_random! :)

	Logins of people that inspired/gave some of these:
	- aroma
	- thivan-d
	- jegoh
	- sholz
	- bdehais
	- jroseiro
	- cczerwin
	- pchojka
	- vivaz-ca
	- anboisve
	- bkarabab
	- akozin
	- scraeyme
*/
constexpr const char	*SPLASH_TEXT_TITLES[SPLASH_TEXT_COUNT] =
{
	"By mbatty and mbirou!",
	"42 Angouleme",
	"Outstanding project!",
	"Is this ft_vox?",
	"Nice so_long!",
	"Also try cub3d!",
	"Chicken jockey!",
	"Dont forget to git push",
	"Better than the original",
	"Core dumped. Segmentation fault.",
	"C++ > all",
	"sudo rm -rf /",
	":(){ :|:& };:",
	"Minecraft 2.0",
	"Including lava chicken",
	"42. It's in the game",
	"Bogos binted?",
	"Aww man",
	"It's not a bug, it's a feature",
	"Is it all c++? Always has been.",
	"Don't launch with valgrind!",
	"Technoblade Never Dies!",
	"Linux",
	"minecra_ft",
	"42 Angouleme > all",
	"No, it's not in rust..."
};

void	TitleScene::onEnter()
{	
	if (_loaded)
		return ;

	_loaded = true;
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	_panel.add("background", new BackgroundImage(textures.get(TX_PATH_DIRT), shaders.get("background"), 0.5));

	UIElement *tmp = _panel.add("options", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED), shaders.get("image"),
													glm::vec2(-51, 40), glm::vec2(0.5), glm::vec2(0.98, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new OptionsScene(_game));
		});
	_panel.add("options_text", new Text("Options", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(-51, 40), glm::vec2(0.5)));

	tmp = _panel.add("quit", new Button(textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED), shaders.get("image"),
										glm::vec2(51, 40), glm::vec2(0.5), glm::vec2(0.98, 1)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			_game->setRunning(false);
		});
	_panel.add("quit_text", new Text("Quit game", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(51, 40), glm::vec2(0.5)));

	tmp = _panel.add("singpleplayer", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
											glm::vec2(0, -22), glm::vec2(0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new SingleplayerScene(_game));
		});
	_panel.add("singleplayer_text", new Text("Singleplayer", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, -22), glm::vec2(0.5)));

	tmp = _panel.add("multiplayer", new Button(textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED), shaders.get("image"),
									glm::vec2(0, 0), glm::vec2(0.5)));
	static_cast<Button*>(tmp)->setClickFunc(
		[this]()
		{
			this->requestScene(new MultiplayerScene(_game));
		});
	_panel.add("multiplayer_text", new Text("Multiplayer", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 0), glm::vec2(0.5)));

	_panel.add("icon", new Image(textures.get(TX_PATH_FT_MINECRAFT), shaders.get("image"),
							glm::vec2(0, -90), glm::vec2(0.5, 0.5), glm::vec2(0.3, 0.3)));

	_panel.add("credits", new Text("By mbatty & mbirou", textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(0, 0), glm::vec2(1)));

	tmp = _panel.add("splash_text", new Text(SPLASH_TEXT_TITLES[rand() % SPLASH_TEXT_COUNT], textures.get(TX_PATH_ASCII), shaders.get("font"), glm::vec2(115, -85), glm::vec2(0.5)));
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

	Text *splashText = static_cast<Text*>(_panel.get("splash_text"));

	splashText->setScale(glm::vec2(1.0 + abs(cos(glfwGetTime() * 4) / 6)));
	if (glfwGetTime() - _lastSplashTextUpdate > SPLASH_TEXT_INTERVAL)
	{
		splashText->setText(SPLASH_TEXT_TITLES[rand() % SPLASH_TEXT_COUNT]);

		_lastSplashTextUpdate = glfwGetTime();
	}
}

void	TitleScene::render()
{
	Window	&window = _game->getWindow();

	glDisable(GL_DEPTH_TEST);

	_panel.draw(window.getSize());
}
