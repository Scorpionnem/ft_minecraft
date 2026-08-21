#pragma once

#include "render/Texture.hpp"
#include "math.hpp"

#include <map>
#include <string>

class   TextureAtlas
{
    public:
        TextureAtlas() {}
        ~TextureAtlas() {}

        TextureAtlas(const TextureAtlas&) = delete;
        TextureAtlas& operator=(const TextureAtlas&) = delete;

        void        add_texture(const std::string& path);

        void        upload();

        void        bind(u32 unit) const {_texture.bind(unit);}

        vec4f       uv(const std::string& path) const;

        const Texture&  texture() const {return (_texture);}
        u32         width() const {return (_width);}
        u32         height() const {return (_height);}

    private:
        Texture     _texture;

        std::vector<u8> _pixels;
        u32             _width = 0;
        u32             _height = 0;
        u32             _channels = 4;

        std::map<std::string, vec4f>   _uvs;
};
