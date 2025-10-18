/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:17:02 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 14:05:25 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TitleScene.hpp"
#include "Game.hpp"
#include "OptionsScene.hpp"
#include "MultiplayerScene.hpp"
#include "SingleplayerScene.hpp"
#include "GameScene.hpp"

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

#include "ImprovedButton.hpp"
#include "ImprovedText.hpp"
#include "ImprovedBackgroundImage.hpp"
#include "ImprovedImage.hpp"
#include "ImprovedToggle.hpp"
#include "ImprovedTextField.hpp"
#include "ImprovedSlider.hpp"
#include "Quad.hpp"

void	TitleScene::onEnter()
{
	if (_loaded)
		return ;

	_loaded = true;
	TextureManager &textures = _game->getTextures();
	ShaderManager &shaders = _game->getShaders();

	// Scene decoration
	_panel.add("background", new ImprovedBackgroundImage(glm::vec2(REFERENCE_WIDTH, REFERENCE_HEIGHT), glm::vec2(0), glm::vec2(0), glm::vec2(1), 0.5, shaders.get("background"), textures.get(TX_PATH_DIRT)));

	_panel.add("icon", new ImprovedImage(
		glm::vec2(textures.get(TX_PATH_FT_MINECRAFT)->getWidth() * 0.3, textures.get(TX_PATH_FT_MINECRAFT)->getHeight() * 0.3),
		glm::vec2(0.5, 0.5), glm::vec2(0, -90), shaders.get("image"), textures.get(TX_PATH_FT_MINECRAFT)));

	_panel.add("credits", new ImprovedText("By mbatty & mbirou", 1, glm::vec2(1), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));

	UIElement *tmp = _panel.add("splash_text", new ImprovedText(SPLASH_TEXT_TITLES[rand() % SPLASH_TEXT_COUNT], 1, glm::vec2(0.5), glm::vec2(140, -80), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedText*>(tmp)->setAngle(-15);
	static_cast<ImprovedText*>(tmp)->setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	// Options button to access options scene
	tmp = _panel.add("options", new ImprovedButton(glm::vec2(98, 20), glm::vec2(0.5), glm::vec2(-51, 40), shaders.get("image"), textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED)));
	_panel.add("options_text", new ImprovedText("Options", 1, glm::vec2(0.5), glm::vec2(-51, 40), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new OptionsScene(_game));
		});

	// Quit button to leave the game
	tmp = _panel.add("quit", new ImprovedButton(glm::vec2(98, 20), glm::vec2(0.5), glm::vec2(51, 40), shaders.get("image"), textures.get(TX_PATH_BUTTON_SMALL), textures.get(TX_PATH_BUTTON_SMALL_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_SMALL_DISABLED)));
	_panel.add("quit_text", new ImprovedText("Quit game", 1, glm::vec2(0.5), glm::vec2(51, 40), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			_game->setRunning(false);
		});

	// Button to go to the singleplayer world selection scene
	tmp = _panel.add("singpleplayer", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5), glm::vec2(0, -22), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	_panel.add("singleplayer_text", new ImprovedText("Singleplayer", 1, glm::vec2(0.5), glm::vec2(0, -22), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new SingleplayerScene(_game));
		});

	// Button to go to the multiplayer server selection scene
	tmp = _panel.add("multiplayer", new ImprovedButton(glm::vec2(200, 20), glm::vec2(0.5), glm::vec2(0), shaders.get("image"), textures.get(TX_PATH_BUTTON), textures.get(TX_PATH_BUTTON_HIGHLIGHTED), textures.get(TX_PATH_BUTTON_DISABLED)));
	_panel.add("multiplayer_text", new ImprovedText("Multiplayer", 1, glm::vec2(0.5), glm::vec2(0), shaders.get("font"), textures.get(TX_PATH_ASCII)));
	static_cast<ImprovedButton*>(tmp)->setClickFunc(
		[this]()
		{
			this->_requestScene(new MultiplayerScene(_game));
		});
}

void	TitleScene::onExit()
{
}

void	TitleScene::processInput(float deltaTime)
{
	(void)deltaTime;
}

void	TitleScene::update(UIEvent events, float deltaTime)
{
	(void)deltaTime;

	_panel.handleEvents(events);

	// Update splash text
	ImprovedText *splashText = static_cast<ImprovedText*>(_panel.get("splash_text"));
	splashText->setScale(1.0 + abs(cos(glfwGetTime() * 4) / 6));
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
