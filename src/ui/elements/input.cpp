#include "ui/UI.hpp"

bool    UI::input(const std::string& label, std::string& input, vec2i pos, vec2i size, Anchor anchor)
{
	vec2i spos;
	vec2i ssize;
    scalePosAndSize(spos, ssize, pos, size, anchor);

    bool	hovered = isOnBox(spos, ssize);
    bool	focused = UI::focused_text_input == label;

    UI::draws.push_back({.pos = spos, .size = ssize, .hovered = focused});

    std::string	render_input = input;
    if (focused)
    	render_input += "_";
    centeredScaledTextY(render_input, pos, size, anchor);

    if (UI::input_ptr->wasPressed(SDL_BUTTON_LEFT))
    {
    	if (focused)
     		UI::focused_text_input = "";
    	else if (hovered)
    		UI::focused_text_input = label;
    }

    const std::string	&text_input = UI::input_ptr->textInput();
    if (!text_input.empty() && focused)
    	input += text_input;

    if (UI::input_ptr->wasPressed(SDLK_BACKSPACE) && focused && !input.empty())
    	input = input.substr(0, input.size() - 1);

    return (false);
}
