/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:35:49 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 10:53:54 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Texture.hpp"

void	Texture::upload()
{
	if (_uploaded)
		return ;

	glGenTextures(1, &_id);

	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, _format, GL_UNSIGNED_BYTE, _pixels.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	_uploaded = true;
}

void	Texture::_missingTexture()
{
	_pixels = {255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255};
	_width = 2;
	_height = 2;
	_format = GL_RGBA;
}

void	Texture::load(const std::string &path)
{
	int	channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path.c_str(), &_width, &_height, &channels, 0);
	if (!data) {
		_missingTexture();
		std::cerr << "Failed to load texture: " << path << "\n";
		return ;
	}

	_format = GL_RGB;
	if (channels == 1)
		_format = GL_RED;
	else if (channels == 3)
		_format = GL_RGB;
	else if (channels == 4)
		_format = GL_RGBA;

	_pixels = std::vector<unsigned char>(data, data + _width * _height * channels);

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
