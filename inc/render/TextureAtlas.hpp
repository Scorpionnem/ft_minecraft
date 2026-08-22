#pragma once

#include "loader/texture/STBLoader.hpp"
#include "render/Texture.hpp"
#include "utils/AABB.hpp"
#include "vec.hpp"

#include <algorithm>
#include <climits>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

class   TextureAtlas
{
    public:
        TextureAtlas() {}
        ~TextureAtlas() {}

        TextureAtlas(const TextureAtlas&) = delete;
        TextureAtlas& operator=(const TextureAtlas&) = delete;

        void        add_texture(const std::string& path)
        {
            int             width;
            int             height;
            int             channels;
            GLenum          format;
            std::vector<u8> data;

            STBLoader::load(path, data, width, height, channels, format);
            add_texture(path, data, width, height, channels, format);
        }
        void	add_texture(const std::string& path, const std::vector<u8>& pixels, int width, int height, int channels, GLenum format)
        {
	        std::vector<u8> rgba = _toRGBA(pixels, width, height, channels);

	        while (width > (int)_size || height > (int)_size)
	            _sizeUp();

	        smallest = std::min({smallest, width, height});

	        vec2i   size = vec2i(width, height);
	        vec2i   pos;

	        int max_tries = 16;
	        for (int tries = 0; tries < max_tries; tries++)
	        {
	            if (_findSpot(size, pos))
	            {
	                _insertTexture(rgba, width, height, pos);
	                _uvs[path] = aabb2i{.min = pos, .max = pos + size - vec2i(1)};
	                _dirty = true;
	                return ;
	            }
	            _sizeUp();
	        }
	        throw std::runtime_error("TextureAtlas: could not find a spot for " + path);
        }

        void        upload()
        {
            if (!_dirty)
                return ;

            _texture.clear_pixel_data();
            _texture.set_format(_size, _size, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
            _texture.add_pixel_data(_pixels.data(), _pixels.size());
            _texture.upload();
            _dirty = false;
        }

        void        bind(u32 unit) const {_texture.bind(unit);}

        bool        has(const std::string& path) const {return (_uvs.find(path) != _uvs.end());}

        vec4f       uv(const std::string& path) const
        {
            auto    it = _uvs.find(path);
            if (it == _uvs.end())
                throw std::runtime_error("TextureAtlas: unknown texture " + path);

            const aabb2i&   box = it->second;
            float           s = (float)_size;

            return (vec4f(box.min.x() / s, box.min.y() / s, (box.max.x() + 1) / s, (box.max.y() + 1) / s));
        }

        const Texture&  texture() const {return (_texture);}
        u32         size() const {return (_size);}

    private:
        bool    _isSpotFree(const aabb2i& target_box) const
        {
            for (auto &[path, box] : _uvs)
                if (aabb2i::intersects(target_box, box))
                    return (false);
            return (true);
        }

        bool    _findSpot(const vec2i& size, vec2i& out_pos)
        {
            for (last_x = 0; last_x + size.x() <= (int)_size; last_x += smallest)
            {
                for (last_y = 0; last_y + size.y() <= (int)_size; last_y += smallest)
                {
                    aabb2i  target_box = {.min = vec2i(last_x, last_y), .max = vec2i(last_x + size.x() - 1, last_y + size.y() - 1)};

                    if (_isSpotFree(target_box))
                    {
                        out_pos = vec2i(last_x, last_y);
                        return (true);
                    }
                }
            }
            return (false);
        }

        void    _sizeUp()
        {
            u32             old_size = _size;
            std::vector<u8> old_pixels = std::move(_pixels);

            _n++;
            _size = 1u << _n;
            _pixels.assign((u64)_size * _size * _channels, 0);

            for (u32 y = 0; y < old_size; y++)
            {
                u8*         dst = &_pixels[(u64)y * _size * _channels];
                const u8*   src = &old_pixels[(u64)y * old_size * _channels];

                std::copy(src, src + (u64)old_size * _channels, dst);
            }
        }

        void    _insertTexture(const std::vector<u8>& rgba, int width, int height, const vec2i& pos)
        {
            for (int y = 0; y < height; y++)
            {
                u8*         dst = &_pixels[((u64)(pos.y() + y) * _size + pos.x()) * _channels];
                const u8*   src = &rgba[(u64)y * width * _channels];

                std::copy(src, src + (u64)width * _channels, dst);
            }
        }

        static std::vector<u8> _toRGBA(const std::vector<u8>& data, int width, int height, int channels)
        {
            std::vector<u8> out((u64)width * height * 4, 255);

            for (u64 i = 0; i < (u64)width * height; i++)
            {
                for (int c = 0; c < channels && c < 4; c++)
                    out[i * 4 + c] = data[i * channels + c];
                if (channels == 1)
                {
                    out[i * 4 + 1] = out[i * 4];
                    out[i * 4 + 2] = out[i * 4];
                }
            }
            return (out);
        }

        Texture     _texture;
        bool        _dirty = false;

        std::vector<u8> _pixels;
        u32             _n = 1; // power of 2 size of the atlas
        u32             _size = 0; // always a square
        u32             _channels = 4;

        int smallest = INT_MAX;

        int last_x = 0;
        int last_y = 0;

        std::map<std::string, aabb2i>   _uvs;
};
