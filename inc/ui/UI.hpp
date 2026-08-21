#pragma once

#include "math.hpp"
#include "platform/Input.hpp"

namespace UI
{
    void    init();
    void    beginFrame(const Input& input);
    void    render();

    void    setTargetSize(u32 width, u32 height);

    bool    button(const std::string& label, vec2i pos, vec2i size);
    // bool    text(const std::string& label, vec2i pos, vec2i size, ...); // va args
    // bool    text_field(const std::string& label, std::string& input, vec2i pos, vec2i size); // va args
};
