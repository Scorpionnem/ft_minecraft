/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:33:09 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 13:10:34 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP

# include "libs.hpp"

# define DEFAULT_WINDOW_WIDTH 860
# define DEFAULT_WINDOW_HEIGHT 520
# define WINDOW_NAME "ft_minecraft"

class	Window
{
	public:
		Window();
		~Window();
		
		void	open(const std::string &name, int width, int height, bool fullScreen);
		void	close();

		void	frameStart();
		void	frameEnd();

		float	getDeltaTime() {return (this->_deltaTime);}
		float	getWidth() {return (this->_width);}
		float	getHeight() {return (this->_height);}
		glm::vec2	getSize() {return (glm::vec2(_width, _height));}
		float	getAspectRatio() {return (this->_width / this->_height);}
		
		glm::vec2	getMousePos()
		{
			double	x, y;

			glfwGetCursorPos(_data, &x, &y);
			return (glm::vec2(x, y));
		}

		void	setWindowPointer(void *ptr);
		void	setWidth(float width) {this->_width = width;}
		void	setHeight(float height) {this->_height = height;}

		bool		up() {return (!glfwWindowShouldClose(_data));}
		GLFWwindow	*data() {return (_data);}
	private:
		static void		_resize(GLFWwindow* window, int width, int height);
		
		float		_deltaTime = 0.0f;
		float		_currentFrame = 0.0f;
		float		_lastFrame = 0.0f;
		float		_width = 0.0f;
		float		_height = 0.0f;
		GLFWwindow	*_data;
};

#endif
