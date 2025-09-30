/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:25:27 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 13:48:50 by mbatty           ###   ########.fr       */
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

class	Game
{
	public:
		Game();
		~Game();

		void	run();

		Window	&getWindow() {return (this->_window);}
		Window	&getCamera() {return (this->_window);}

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
		void	_updateCamera(float deltaTime);
		
		void	_render();

		bool	_running = true;
		Window	_window;
		Input	_input;

		Camera	_camera;

		TextureManager	_textures;
		ShaderManager	_shaders;

		UIElement	*testButton;
};

#endif
