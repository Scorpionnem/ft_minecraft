/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:35:49 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 15:55:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Texture.hpp"

void	Texture::upload(unsigned char *pixels, int width, int height, GLenum format)
{
	glGenTextures(1, &_id);

	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void	Texture::_missingTexture()
{
	unsigned char pixels[16] = {255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255};
	int width = 2;
	int height = 2;
	
	_width = 16;
	_height = 16;

	upload(pixels, width, height, GL_RGBA);
}

void	Texture::load(const std::string &path)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path.c_str(), &_width, &_height, &_channels, 0);
	if (!data) {
		_missingTexture();
		std::cerr << "Failed to load texture: " << path << "\n";
		return ;
	}

	GLenum format = GL_RGB;
	if (_channels == 1)
		format = GL_RED;
	else if (_channels == 3)
		format = GL_RGB;
	else if (_channels == 4)
		format = GL_RGBA;
		
	upload(data, _width, _height, format);

	stbi_image_free(data);
}

void	Texture::unload()
{
	if (_id)
		glDeleteTextures(1, &_id);
	_id = 0;
}

void	Texture::bind(int offset)
{
	glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GL_TEXTURE_2D, _id);
}
