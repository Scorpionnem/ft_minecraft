#pragma once

#include "math.hpp"
#include "platform/Input.hpp"
#include "render/Font.hpp"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
#include "render/Mesh.hpp"
#include "utils/AABB.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

class UI
{
    public:
        enum class Anchor
        {
            CENTER,
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_LEFT,
            BOTTOM_RIGHT,
        };

        static void    init(const std::string& font_path);
        static void	destroy();
        static void    beginFrame(const Input& input);
        static void    render();

        static void    setTargetSize(u32 width, u32 height);
        static void    setTargetFontScale(u32 scale);

        static float	getScale();
        static u32	getFontSizeY();
        static u32	getFontSizeX(char c);
        static u32	getFontSizeX(const std::string &s);

        static void	text(const std::string& label, vec2i pos, Anchor anchor = Anchor::CENTER, vec3f color = vec3f(1), bool background = false, vec3f background_color = vec3f(1));
        // takes advance [0, 1]
        static void	progress_bar(float advance, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
        static bool    button(const std::string& label, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
        static bool	toggle(const std::string& label, bool& state, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
        static bool    input(const std::string& label, std::string& input, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
        static bool	slider(const std::string& label, int& input, int min, int max, vec2i pos, vec2i size, Anchor anchor = Anchor::CENTER);
    private:
        static vec2i	anchorOrigin(vec2i ssize, UI::Anchor anchor);
        static bool	isOnBox(vec2i spos, vec2i ssize);
        static void	scalePosAndSize(vec2i& spos, vec2i& ssize, vec2i pos, vec2i size, UI::Anchor anchor);
        static void	centeredScaledTextY(const std::string& label, vec2i pos, vec2i size, UI::Anchor anchor);
        static void	centeredScaledText(const std::string& label, vec2i pos, vec2i size, UI::Anchor anchor);
    private:
        struct  DrawInfo
        {
            vec2i   pos;
            vec2i   size;
            bool	hovered;
            bool	textured = false;
            vec4f	uv = vec4f(0, 0, 1, 1);
            bool	text = false;
            bool	text_background = false;
            vec3f	text_color = vec3f(1);
            vec3f	text_background_color = vec3f(1);
        };
        struct	TextDrawInfo
        {
	        vec2i   pos;
			vec2i   size;
			vec4f	uv = vec4f(0, 0, 1, 1);

			bool	background = false;
            vec3f	color = vec3f(1);
            vec3f	background_color = vec3f(1);
        };
    private:
        static const Input* input_ptr;

        static u32 target_width;
        static u32 target_height;
        static u32 target_font_scale;

        static float    scale;
        static float    offset_x;
        static float    offset_y;

        static Mesh    rect_mesh;
        static Shader  rect_shader;
        static Shader	text_shader;

        static Font	font;

        static std::vector<DrawInfo>  draws;
        static std::vector<TextDrawInfo>  text_draws;
        static std::string	focused_text_input;
        static std::string	dragging_slider;
};
