/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIElement.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:05:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 14:52:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UIELEMENT_HPP
# define UIELEMENT_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Texture.hpp"
# include "Window.hpp"
# include "Input.hpp"

constexpr float REFERENCE_WIDTH = DEFAULT_WINDOW_WIDTH / 2;
constexpr float REFERENCE_HEIGHT = DEFAULT_WINDOW_HEIGHT / 2;

struct UIEvent
{
	glm::vec2		windowSize = glm::vec2(0.0f);
	glm::vec2		mousePos = glm::vec2(0.0f);
	Input			*inputs;
	int				scroll = 0;
};

struct Vertex2D
{
	glm::vec2 pos;
	glm::vec2 uv;
};

constexpr float quadVertices[] =
{
	0.0f, 0.0f,  0.0f, 0.0f,
	1.0f, 1.0f,  1.0f, 1.0f,
	0.0f, 1.0f,  0.0f, 1.0f,
	0.0f, 0.0f,  0.0f, 0.0f,
	1.0f, 0.0f,  1.0f, 0.0f,
	1.0f, 1.0f,  1.0f, 1.0f
};

/*
	UIElement interface to implement all types of ui elements
*/
class UIElement
{
	public:
		UIElement() {}
		virtual ~UIElement()
		{
			glDeleteVertexArrays(1, &_VAO);
			glDeleteBuffers(1, &_VBO);
		}

		/*
			Updates the given UIElement, could be clicking, scrolling...

			@param events UIEvent struct that holds all kinds of events (mouseScroll, mousePos...)
		*/
		virtual void	handleEvents(UIEvent events) = 0;
		/*
			Draws the UIElement using the given shader and to scale of the windowSize

			@param shader shader used to draw the UIElement
			@param windowSize window size to scale the UIElement
		*/
		virtual void	draw(Shader *shader, glm::vec2 windowSize) = 0;

		/*
			Returns the UI scale
			
			@param windowSize size of the window
		*/
		static float	getUiScale(glm::vec2 windowSize)
		{
			float windowWidth  = (float)windowSize.x;
			float windowHeight = (float)windowSize.y;
		
			float scaleX = windowWidth / REFERENCE_WIDTH;
			float scaleY = windowHeight / REFERENCE_HEIGHT;
		
			return (std::min(scaleX, scaleY));
		}
	protected:
		void _upload(void)
		{
			if (_VAO != 0)
				return ;

			glGenVertexArrays(1, &_VAO);
			glGenBuffers(1, &_VBO);

			glBindVertexArray(_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			glBindVertexArray(0);
		}
		uint		_VAO = 0;
		uint		_VBO = 0;
		
		static bool	_isInBounds(glm::vec2 point, glm::vec2 zonePos, glm::vec2 zoneSize)
		{
			return (point.x >= zonePos.x && point.x <= zonePos.x + zoneSize.x && point.y >= zonePos.y && point.y <= zonePos.y + zoneSize.y);
		}
		static glm::vec2	_getScaledPos(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, glm::vec2 windowSize)
		{
			float	scale = UIElement::getUiScale(windowSize);
			glm::vec2	scaledSize = size * scale;
			
			float	x = (anchor.x * windowSize.x) - (anchor.x * scaledSize.x);
			float	y = (anchor.y * windowSize.y) - (anchor.y * scaledSize.y);
			
			return (glm::vec2(x, y) + (offset * scale));
		}
};

#endif
