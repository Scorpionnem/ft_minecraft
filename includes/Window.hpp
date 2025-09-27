/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:33:09 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 13:20:12 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP

# include "libs.hpp"

# define DEFAULT_WINDOW_WIDTH 860
# define DEFAULT_WINDOW_HEIGHT 520
# define DEFAULT_WINDOW_NAME "ft_minecraft"

# define FULL_SCREEN 0

class	Window
{
	public:
		Window();
		~Window();
		
		void	open();
		void	close();

		void	frameStart();
		void	frameEnd();

		float	getDeltaTime() {return (this->_deltaTime);}
		bool	up() {return (!glfwWindowShouldClose(_data));}
		GLFWwindow	*data() {return (_data);}
	private:
		float		_deltaTime = 0.0f;
		float		_currentFrame = 0.0f;
		float		_lastFrame = 0.0f;
		float		_width = 0.0f;
		float		_height = 0.0f;
		GLFWwindow	*_data;
};

#endif
