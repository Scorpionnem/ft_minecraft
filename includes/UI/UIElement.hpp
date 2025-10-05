/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIElement.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:05:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 11:43:21 by mbatty           ###   ########.fr       */
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

class UIElement
{
	public:
		UIElement() {}
		virtual ~UIElement()
		{
			glDeleteVertexArrays(1, &_VAO);
			glDeleteBuffers(1, &_VBO);
		}

		virtual void	handleEvents(UIEvent events) = 0;
		virtual void	draw(glm::vec2 windowSize) = 0;

		virtual void	setSize(glm::vec2 size) {_size = size;}
		virtual void	setSizeX(float size) {_size.x = size;}
		virtual void	setSizeY(float size) {_size.y = size;}

		virtual void	setAnchor(glm::vec2 anchor) {_anchor = anchor;}
		virtual void	setAnchorX(float anchor) {_anchor.x = anchor;}
		virtual void	setAnchorY(float anchor) {_anchor.y = anchor;}

		virtual void	setOffset(glm::vec2 offset) {_offset = offset;}
		virtual void	setOffsetX(float offset) {_offset.x = offset;}
		virtual void	setOffsetY(float offset) {_offset.y = offset;}

		virtual glm::vec2	getSize() {return (_size);}
		virtual glm::vec2	getAnchor() {return (_anchor);}
		virtual glm::vec2	getOffset() {return (_offset);}
	protected:
		void		_upload(void);
		uint		_VAO = 0;
		uint		_VBO = 0;

		glm::vec2			_size = glm::vec2(0);
		glm::vec2			_anchor = glm::vec2(0.5);
		glm::vec2			_offset = glm::vec2(0);

		float				_scale = 0;
		glm::vec2			_pos;

		Shader				*_shader;

		static bool			_isInBounds(glm::vec2 point, glm::vec2 zonePos, glm::vec2 zoneSize);
		static glm::vec2	_getScaledPos(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, glm::vec2 windowSize);
		static float		_getUiScale(glm::vec2 windowSize);
		static glm::vec2	_getUiScaleXY(glm::vec2 windowSize);
};

#endif
