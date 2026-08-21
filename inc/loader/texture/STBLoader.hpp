#pragma once

#include "render/Texture.hpp"

#include <vector>

namespace	STBLoader
{
    void    load(const std::string& path, std::vector<u8>& pixels, int& width, int& height, int& channels, GLenum& format);
	void	load(const std::string& path, Texture& texture);
}
