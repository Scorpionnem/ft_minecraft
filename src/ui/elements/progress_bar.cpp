#include "ui/UI.hpp"

void	UI::progress_bar(float advance, vec2i pos, vec2i size, Anchor anchor)
{
	advance = std::clamp(advance, 0.0f, 1.0f);

	vec2i spos;
    vec2i ssize;
    scalePosAndSize(spos, ssize, pos, size, anchor);

    UI::draws.push_back({.pos = spos, .size = ssize, .hovered = false});

    scalePosAndSize(spos, ssize, pos, vec2i(size.x() * advance, size.y()), anchor);

    UI::draws.push_back({.pos = spos, .size = ssize, .hovered = true});
}
