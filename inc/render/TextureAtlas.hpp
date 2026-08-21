#pragma once

#include "render/Texture.hpp"
#include "utils/AABB.hpp"

#include <map>
#include <string>

/*
 * when you add a texture, it looks for a free spot
 * a free spot does not intersect with any other texture's spot
 * a spot is a rectangle containing the texture
 *
 * if it cant find a spot:
 * increase _n by 1
 * alloc new vector for pixels
 * copy all old data in the same spots as before
 * find a spot to fit texture
 */

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
        u32         size() const {return (_size);}

    private:
        bool    isSpotFree(const aabb2i& target_box)
        {
            for (auto &[path, box] : _uvs)
                if (aabb2i::intersects(target_box, box))
                    return (false);
            return (true);
        }

        Texture     _texture;

        std::vector<u8> _pixels;
        u32             _n = 1; // power of 2 size of the atlas
        u32             _size = 0; // always a square
        u32             _channels = 4;

        std::map<std::string, aabb2i>   _uvs;
};
