#include "ui/UI.hpp"

void	UI::text(const std::string& label, vec2i pos, Anchor anchor)
{
	int	char_size = UI::font.get_char_size();
	float	char_scale = target_font_scale * UI::scale;

	int	total_width = UI::font.get_width(label);

	vec2i	origin = anchorOrigin(vec2i(total_width * char_scale, char_size * char_scale), anchor);
	float	base_x = pos.x() * UI::scale + origin.x();
	float	base_y = pos.y() * UI::scale + origin.y();
	float	cursor_x = base_x;

	for (char c : label)
	{
		int	width = UI::font.get_width(c);
		std::string key(1, c);

		if (UI::font.get_atlas().has(key))
		{
			vec2i spos = vec2i(cursor_x, base_y);
			vec2i ssize = vec2i(width * char_scale, char_size * char_scale);

			UI::draws.push_back({.pos = spos, .size = ssize, .hovered = false, .textured = true, .uv = UI::font.get_atlas().uv(key)});
		}

		cursor_x += width * char_scale;
	}
}
