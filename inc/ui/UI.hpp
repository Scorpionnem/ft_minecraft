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

    u32	getFontSizeY();
    u32	getFontSizeX(char c);
    u32	getFontSizeX(const std::string &s);

    bool    button(const std::string& label, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
    void	text(const std::string& label, vec2i pos, Anchor anchor = Anchor::CENTER);
    // takes state of the toggle as input, returns true when toggle is clicked
    bool	toggle(const std::string& label, bool& state, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
    bool    input(const std::string& label, std::string& input, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
    bool	slider(const std::string& label, int& input, int min, int max, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
}
