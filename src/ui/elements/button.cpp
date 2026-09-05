#include "ui/UI.hpp"

bool    UI::button(const std::string& label, vec2i pos, vec2i size, Anchor anchor)
{
    vec2i spos;
    vec2i ssize;
    scalePosAndSize(spos, ssize, pos, size, anchor);

    bool	hovered = isOnBox(spos, ssize);

    UI::draws.push_back({.pos = spos, .size = ssize, .hovered = hovered});

    centeredScaledText(label, pos, size, anchor);

    if (UI::input_ptr->wasPressed(SDL_BUTTON_LEFT) && hovered)
        return (true);
    return (false);
}
