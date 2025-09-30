/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Image.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:29:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 02:04:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_HPP
# define IMAGE_HPP

# include "libs.hpp"
# include "Shader.hpp"
# include "Texture.hpp"

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

enum class Anchor
{
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		CENTER_LEFT,
		CENTER,
		CENTER_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT
};

/*

	TODO:
		Make the constructor take an anchor point and offset from it
		Make the draw function only take a shader and scale.

		Add width/height to the class instead of always taking from the texture.
*/

class Image
{
	public:
		Image(Texture* texture)
			: _texture(texture)
		{
			_upload();
		}

		~Image()
		{
			glDeleteVertexArrays(1, &_VAO);
			glDeleteBuffers(1, &_VBO);
		}

		void draw(glm::vec2 pos, Shader* shader, float scale)
		{
			draw(pos, shader, glm::vec2(_texture->getWidth(), _texture->getHeight()) * scale);
		}
		void draw(glm::vec2 pos, Shader* shader, glm::vec2 scale)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f))
							* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

			shader->bind();
			shader->setMat4("model", model);
			shader->setInt("tex", 0);

			_texture->bind(0);

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
		}

		float	getWidth(float scale) {
			return (_texture->getWidth() * scale);
		}
		float	getHeight(float scale) {
			return (_texture->getHeight() * scale);
		}
	private:
		void _upload(void)
		{
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

		Texture	*_texture;
		uint	_VAO;
		uint	_VBO;
		uint	_vertsSize;
};

#endif
