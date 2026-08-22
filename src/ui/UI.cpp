#include "ui/UI.hpp"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "render/Mesh.hpp"
#include "utils/AABB.hpp"
#include "render/Font.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace UI
{
    static const Input* input_ptr;

    static u32 target_width;
    static u32 target_height;
    static u32 target_font_scale = 1;

    static float    scale;
    static float    offset_x;
    static float    offset_y;

    static Mesh    rect_mesh;
    static Shader  rect_shader;

    static Font	font;

    struct  DrawInfo
    {
        vec2i   pos;
        vec2i   size;
        bool	hovered;
        bool	textured = false;
        vec4f	uv = vec4f(0, 0, 1, 1);
    };

    static std::vector<DrawInfo>  draws;
    static std::string	focused_text_input;
    static std::string	dragging_slider;
}

void	UI::setTargetFontScale(u32 scale)
{
	UI::target_font_scale = scale;
}

void    UI::setTargetSize(u32 width, u32 height)
{
    UI::target_width = width;
    UI::target_height = height;
}

void	UI::destroy()
{
}

void    UI::init(const std::string& font_path)
{
	font.load(font_path, 16);
	font.upload();

    vec2f rect_verts[] =
	{
		{0.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f}
	};

	UI::rect_mesh.set_sizeof_layout(sizeof(vec2f));
	UI::rect_mesh.add_vertex_layout(0, 2, GL_FLOAT, 0);
	UI::rect_mesh.add_vertex_data(reinterpret_cast<u8*>(rect_verts), sizeof(rect_verts));
	UI::rect_mesh.upload();
	UI::rect_shader.load("assets/shaders/ui/rect.vert", "assets/shaders/ui/rect.frag");
}

static vec2i	anchorOrigin(vec2i ssize, UI::Anchor anchor)
{
    switch (anchor)
    {
        case UI::Anchor::TOP_LEFT:
            return (vec2i(0, 0));
        case UI::Anchor::TOP_RIGHT:
            return (vec2i(UI::input_ptr->width() - ssize.x(), 0));
        case UI::Anchor::BOTTOM_LEFT:
            return (vec2i(0, UI::input_ptr->height() - ssize.y()));
        case UI::Anchor::BOTTOM_RIGHT:
            return (vec2i(UI::input_ptr->width() - ssize.x(), UI::input_ptr->height() - ssize.y()));
        default:
            return (vec2i(UI::offset_x, UI::offset_y));
    }
}

void    UI::beginFrame(const Input& input)
{
    UI::input_ptr = &input;

    float scaleX = static_cast<float>(UI::input_ptr->width()) / UI::target_width;
    float scaleY = static_cast<float>(UI::input_ptr->height()) / UI::target_height;
    UI::scale = std::min(scaleX, scaleY);
    UI::offset_x = (UI::input_ptr->width()  - UI::target_width  * UI::scale) * 0.5f;
    UI::offset_y = (UI::input_ptr->height() - UI::target_height * UI::scale) * 0.5f;
}

void    UI::render()
{
    for (UI::DrawInfo &d : UI::draws)
    {
        const vec2i &pos = d.pos;
        const vec2i &size = d.size;

        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        UI::rect_shader.bind();
        mat4f model = mat4f::translate(vec3f(pos.x(), pos.y(), 0.0f)) * mat4f::scale(vec3f(size.x(), size.y(), 1.0f));
        UI::rect_shader.setMat4("uModel", model);
        UI::rect_shader.setMat4("uProj", mat4f::ortho(0.0f, UI::input_ptr->width(), UI::input_ptr->height(), 0.0f, -1.0f, 1.0f));
        UI::rect_shader.setInt("uTex", 0);
        UI::rect_shader.setInt("uUseTex", d.textured ? 1 : 0);

        if (d.textured)
        {
            UI::font.get_atlas().bind(0);
            UI::rect_shader.setVec4("uUV", d.uv);
            UI::rect_shader.setVec3("uColor", vec3f(1, 1, 1));
        }
        else
        {
            UI::rect_shader.setVec3("uColor", d.hovered ? vec3f(0, 1, 0) : vec3f(1, 0, 0));
        }

        UI::rect_mesh.draw();
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
    }
    draws.clear();
}

static void	centeredScaledText(const std::string& label, vec2i pos, vec2i size, UI::Anchor anchor)
{
	vec2i	text_pos;
    text_pos.x() = pos.x() + size.x() / 2 - (UI::font.get_width(label) * (int)UI::target_font_scale) / 2;
    text_pos.y() = pos.y() + size.y() / 2 - (UI::font.get_char_size() * (int)UI::target_font_scale) / 2;
    UI::text(label, text_pos, anchor);
}

static void	centeredScaledTextY(const std::string& label, vec2i pos, vec2i size, UI::Anchor anchor)
{
	vec2i	text_pos;
    text_pos.x() = pos.x();
    text_pos.y() = pos.y() + size.y() / 2 - (UI::font.get_char_size() * (int)UI::target_font_scale) / 2;
    UI::text(label, text_pos, anchor);
}

static void	scalePosAndSize(vec2i& spos, vec2i& ssize, vec2i pos, vec2i size, UI::Anchor anchor)
{
	ssize = vec2i(size.x() * UI::scale, size.y() * UI::scale);
    vec2i origin = anchorOrigin(ssize, anchor);
    spos = vec2i(pos.x() * UI::scale + origin.x(), pos.y() * UI::scale + origin.y());
}

static bool	isOnBox(vec2i spos, vec2i ssize)
{
	aabb2i  box = {.min = spos, .max = spos + ssize};

	return (aabb2i::contains(box, vec2i(UI::input_ptr->mouseX(), UI::input_ptr->mouseY())));
}

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

bool	UI::toggle(const std::string& label, bool& state, vec2i pos, vec2i size, Anchor anchor)
{
    vec2i spos;
	vec2i ssize;
    scalePosAndSize(spos, ssize, pos, size, anchor);

    bool	hovered = isOnBox(spos, ssize);

    UI::draws.push_back({.pos = spos, .size = ssize, .hovered = state});

    centeredScaledText(label, pos, size, anchor);

    if (UI::input_ptr->wasPressed(SDL_BUTTON_LEFT) && hovered)
    {
    	state = !state;
     	return (true);
    }

    return (false);
}

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

bool	UI::slider(const std::string& label, int& input, int min, int max, vec2i pos, vec2i size, Anchor anchor)
{
	vec2i spos;
	vec2i ssize;
    scalePosAndSize(spos, ssize, pos, size, anchor);

    float	frac = (max != min) ? static_cast<float>(input - min) / static_cast<float>(max - min) : 0.0f;
    frac = std::clamp(frac, 0.0f, 1.0f);

    int	handleWidth = std::max(1, size.x() / 10);
    vec2i	sliderpos = vec2i(pos.x() + static_cast<int>(std::round(frac * (size.x() - handleWidth))), pos.y());
    vec2i	slidersize = vec2i(handleWidth, size.y());

    vec2i	sliderspos;
    vec2i	sliderssize;
    scalePosAndSize(sliderspos, sliderssize, sliderpos, slidersize, anchor);

    bool	hovered = isOnBox(spos, ssize);
    bool	dragging = UI::dragging_slider == label;

    UI::draws.push_back({.pos = spos, .size = ssize, .hovered = false});
    UI::draws.push_back({.pos = sliderspos, .size = sliderssize, .hovered = hovered || dragging});

    centeredScaledText(std::to_string(input), pos, size, anchor);

    if (UI::input_ptr->isDown(SDL_BUTTON_LEFT))
    {
    	if (!dragging && hovered && UI::dragging_slider.empty())
    	{
    		UI::dragging_slider = label;
    		dragging = true;
    	}

    	if (dragging)
    	{
    		float	offset = (static_cast<float>(UI::input_ptr->mouseX()) - static_cast<float>(spos.x())) / static_cast<float>(ssize.x());
    		offset = std::clamp(offset, 0.0f, 1.0f);

    		input = min + static_cast<int>(std::round((max - min) * offset));
    	}
    }
    else if (dragging)
    	UI::dragging_slider = "";

    return (false);
}

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
