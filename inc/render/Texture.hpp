#pragma once

#include "math.hpp"
#include "render/GL.hpp"

#include <vector>

class	Texture
{
	public:
		Texture() {}
		~Texture()
		{
			if (_id != 0)
				glDeleteTextures(1, &_id);
		}

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		void	add_pixel_data(u8* bytes, u64 size)
		{
			_pixels.insert(_pixels.end(), bytes, bytes + size);
		}
		void	clear_pixel_data()
		{
			_pixels.clear();
		}
		void	set_format(u32 width, u32 height, GLenum internal_format = GL_RGBA8, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE)
		{
			_width = width;
			_height = height;
			_internal_format = internal_format;
			_format = format;
			_type = type;
		}

		void	upload()
		{
		    if (_pixels.empty())
				_generateUnknownTexture();

			if (_id != 0)
				glDeleteTextures(1, &_id);

			glGenTextures(1, &_id);
			glBindTexture(GL_TEXTURE_2D, _id);

			glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, _width, _height, 0, _format, _type, _pixels.data());
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if (_format == GL_RED)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		void	bind(u32 unit) const
		{
			Texture::bind(_id, unit);
		}
		static void	bind(GLuint id, u32 unit)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, id);
		}

		u32		id() const {return (_id);}
		u32		width() const {return (_width);}
		u32		height() const {return (_height);}
	private:
	    void    _generateUnknownTexture()
		{
    		_pixels = {255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0, 255, 255};
    		_width = 2;
    		_height = 2;
    		_format = GL_RGBA;
		}
		u32				_id = 0;

		std::vector<u8>	_pixels;
		u32				_width = 0;
		u32				_height = 0;

		GLenum			_internal_format = GL_RGBA8;
		GLenum			_format = GL_RGBA;
		GLenum			_type = GL_UNSIGNED_BYTE;
};
