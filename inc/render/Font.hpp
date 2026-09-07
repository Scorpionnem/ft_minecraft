#pragma once

#include "loader/texture/STBLoader.hpp"
#include "render/TextureAtlas.hpp"

#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <string>

class	Font
{
	public:
		Font() {}
		~Font() {}

		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		void	load(const std::string& path, u32 font_atlas_format)
		{
			int             font_width;
			int             font_height;
			int             font_channels;
			GLenum          font_format;
			std::vector<u8> font_pixels;

			STBLoader::load(path, font_pixels, font_width, font_height, font_channels, font_format);

            if (font_width != font_height)
            	throw std::runtime_error("font texture not square " + path);
            if (font_atlas_format == 0)
           		throw std::runtime_error("font_atlas_format cant be 0");

            char_size = font_width / font_atlas_format;

            u32	total = font_atlas_format * font_atlas_format;

		    for (u32 c = 0; c < total; c++)
			{
				if (!std::isprint((int)c))
					continue ;

				u32	col = c % font_atlas_format;
				u32	row = c / font_atlas_format;

				u32	buffer_row = font_atlas_format - 1 - row;

				std::vector<u8> cell = _extractTexture(font_pixels, font_width, font_channels, col * char_size, buffer_row * char_size);

				int	left;
				int	width = _measureWidth(cell, font_channels, left);

				widths[(u8)c] = width;

				if (c == ' ')
					left = 0;
				else if (left >= char_size)
					continue ;

				std::vector<u8> char_data = _cropTexture(cell, font_channels, left, width);
				atlas.add_texture(std::string(1, (char)c), char_data, width, char_size, font_channels, font_format);
			}
		}
		void	upload()
		{
			atlas.upload();
		}

		const TextureAtlas&	get_atlas() const {return (atlas);}
		int	get_char_size() const {return (char_size);}
		int	get_width(char c) const {return (widths[(u8)c]);}
		int	get_width(const std::string& s) const
		{
			int	res = 0;
			for (char c : s)
				res += get_width(c);
			return (res);
		}

	private:
		std::vector<u8>	_extractTexture(const std::vector<u8>& pixels, int stride_width, int channels, int off_x, int off_y)
		{
			std::vector<u8> out((u64)char_size * char_size * channels);

			for (int y = 0; y < char_size; y++)
			{
				const u8*	src = &pixels[((u64)(off_y + y) * stride_width + off_x) * channels];
				u8*			dst = &out[(u64)y * char_size * channels];

				std::copy(src, src + (u64)char_size * channels, dst);
			}
			return (out);
		}

		bool	_isEmptyColumn(const std::vector<u8>& cell, int channels, int x)
		{
			bool	has_alpha = (channels == 2 || channels == 4);
			int		alpha_index = channels - 1;

			for (int y = 0; y < char_size; y++)
			{
				const u8*	px = &cell[((u64)y * char_size + x) * channels];

				if (has_alpha)
				{
					if (px[alpha_index] != 0)
						return (false);
				}
				else
				{
					for (int c = 0; c < channels; c++)
						if (px[c] != 0)
							return (false);
				}
			}
			return (true);
		}

		int	_measureWidth(const std::vector<u8>& cell, int channels, int& out_left)
		{
			int	left = 0;
			while (left < char_size && _isEmptyColumn(cell, channels, left))
				left++;

			if (left == char_size)
			{
				out_left = char_size;
				return (char_size / 2);
			}

			int	right = char_size - 1;
			while (right > left && _isEmptyColumn(cell, channels, right))
				right--;

			out_left = left;
			return (right - left + 2);
		}

		std::vector<u8>	_cropTexture(const std::vector<u8>& cell, int channels, int left, int width)
		{
			std::vector<u8> out((u64)width * char_size * channels);

			for (int y = 0; y < char_size; y++)
			{
				const u8*	src = &cell[((u64)y * char_size + left) * channels];
				u8*			dst = &out[(u64)y * width * channels];

				std::copy(src, src + (u64)width * channels, dst);
			}
			return (out);
		}

		TextureAtlas	atlas;
		int				char_size = 0;
		int				widths[256] = {};
};
