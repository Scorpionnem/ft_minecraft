/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:28:00 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 14:51:55 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

Game::Game() {}
Game::~Game() {}

void	Game::run()
{
	_init();
	while (_running)
	{
		_processInput();
		_update(_window.getDeltaTime());
		_render();
	}
	_stop();
}

void	Game::_init()
{
	_window.open();

	_shaders.load("font", "shaders/text.vertex", "shaders/text.fragment");
	_textures.load("ascii", "assets/textures/ui/font/ascii.png");
	_shaders.get("font")->setInt("tex0", 0);
}

void	Game::_stop()
{
	_window.close();
}

void	Game::_processInput()
{
	_window.frameStart();
	
	_input.update(_window.data());

	if (_input.isKeyPressed(GLFW_KEY_ESCAPE))
		_running = false;
}

void	Game::_update(float deltaTime)
{
	if (!_window.up())
		_running = false;
	(void)deltaTime;
}

uint fontVAO = 0;
uint fontVBO = 0;

void	initFontModel()
{
    if (fontVAO != 0) return;

    float vertices[] = {
        0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 1.0f,  0.0f, 1.0f,
        0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &fontVAO);
    glGenBuffers(1, &fontVBO);

    glBindVertexArray(fontVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void	Game::drawChar(char c, int x, int y)
{
	initFontModel();
	_shaders.get("font")->bind();
	_textures.get("ascii")->bind(0);

	glm::mat4   model(1);

    model = glm::translate(model, glm::vec3(x, y, 1.0f));
	model = glm::scale(model, glm::vec3(100, 100, 1));
    glm::mat4 projection = glm::ortho(0.0f, (float)DEFAULT_WINDOW_WIDTH, (float)DEFAULT_WINDOW_HEIGHT, 0.0f);

	_shaders.get("font")->setMat4("model", model);
	_shaders.get("font")->setMat4("projection", projection);
	_shaders.get("font")->setVec3("color", glm::vec3(1));
	_shaders.get("font")->setInt("charIndex", (int)c);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	glBindVertexArray(fontVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void	Game::_render()
{
	std::string fps = std::to_string((int)(1.0 / _window.getDeltaTime())) + " fps";
	int i = 0;
	for (char c : fps)
		drawChar(c, i++ * 100, 0);

	_window.frameEnd();
}
