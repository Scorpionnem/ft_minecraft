/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:25:27 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 12:51:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

# include "libs.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include "Camera.hpp"
# include "TextureManager.hpp"
# include "ShaderManager.hpp"
# include "UIElement.hpp"
# include "Scene.hpp"

class	Game
{
	public:
		Game();
		~Game();

		void	run();

		Window	&getWindow() {return (this->_window);}
		Input	&getInput() {return (this->_input);}
		TextureManager	&getTextures() {return (this->_textures);}
		ShaderManager	&getShaders() {return (this->_shaders);}

		void	setRunning(bool state)
		{
			_running = state;
		}
	private:
		void	_init();
		void	_loadTextures();
		void	_loadShaders();
		void	_stop();

		void	_processInput();

		void	_update(float deltaTime);

		void	_render();

		void	_swapScene(Scene *scene);

		bool	_running = true;
		Window	_window;
		Input	_input;

		TextureManager	_textures;
		ShaderManager	_shaders;

		Scene	*_currentScene;
		std::map<std::string, Scene*>	_scenes;
};

#endif
