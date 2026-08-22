#pragma once

#include "math.hpp"
#include "platform/Input.hpp"

namespace UI
{
    enum class Anchor
    {
        CENTER,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
    };

    void    init(const std::string& font_path);
    void	destroy();
    void    beginFrame(const Input& input);
    void    render();

    void    setTargetSize(u32 width, u32 height);
    void    setTargetFontScale(u32 scale);

    bool    button(const std::string& label, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
    void	text(const std::string& label, vec2i pos, Anchor anchor = Anchor::CENTER);
    // bool	toggle(const std::string& label, vec2i pos, vec2i size, bool default_state = false, Anchor anchor = Anchor::CENTER);
    // bool    text(const std::string& label, vec2i pos, vec2i size, ...); // va args
    // bool    text_field(const std::string& label, std::string& input, vec2i pos, vec2i size); // va args
};
