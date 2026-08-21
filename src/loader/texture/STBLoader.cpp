#include "loader/texture/STBLoader.hpp"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

void    STBLoader::load(const std::string& path, std::vector<u8>& pixels, int& width, int& height, int& channels, GLenum& format)
{
    stbi_set_flip_vertically_on_load(true);
	u8*	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data)
		throw std::runtime_error("Failed to load texture " + path);

	format = GL_RGB;
	if (channels == 1)
		format = GL_RED;
	else if (channels == 3)
		format = GL_RGB;
	else if (channels == 4)
		format = GL_RGBA;

	pixels.assign(data, data + (width * height * channels));
	stbi_image_free(data);
}

void	STBLoader::load(const std::string& path, Texture& texture)
{
	int		width;
	int		height;
	int		channels;
	GLenum	format;
	std::vector<u8> data;

	STBLoader::load(path, data, width, height, channels, format);
	texture.set_format(width, height, format, format, GL_UNSIGNED_BYTE);
	texture.add_pixel_data(data.data(), width * height * channels);
}
