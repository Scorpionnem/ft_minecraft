/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:25:27 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 14:39:04 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

# include "libs.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include "TextureManager.hpp"
# include "ShaderManager.hpp"

class	Game
{
	public:
		Game();
		~Game();

void	drawChar(char c, int x, int y);

		void	run();
	private:
		void	_init();
		void	_stop();

		void	_processInput();
		void	_update(float deltaTime);
		void	_render();

		bool	_running = true;
		Window	_window;
		Input	_input;

		TextureManager	_textures;
		ShaderManager	_shaders;
};

#endif
