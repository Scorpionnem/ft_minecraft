/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BackgroundImage.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 01:29:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 16:10:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BACKGROUNDIMAGE_HPP
# define BACKGROUNDIMAGE_HPP

# include "UIElement.hpp"

/*
	Text ui element, self explanatory, just a texture on the screen
*/
class BackgroundImage : public UIElement
{
	public:
		BackgroundImage(Texture *texture)
			: _texture(texture)
		{
			_size = glm::vec2(0);
		}

		~BackgroundImage() {}

		void	handleEvents(UIEvent) {}
		void	draw(Shader *shader, glm::vec2 windowSize)
		{
			_upload();
			
			_size = windowSize;
			glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(_size.x, _size.y, 1.0f));

			shader->bind();
			shader->setMat4("model", model);
			shader->setInt("tex", 0);
			shader->setFloat("scale", UIElement::getUiScale(windowSize));

			_texture->bind(0);

			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, sizeof(quadVertices));
		}
	private:

		Texture		*_texture;
		glm::vec2	_size;
};

#endif
