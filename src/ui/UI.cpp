#include "ui/UI.hpp"
#include "SDL_mouse.h"
#include "loader/texture/STBLoader.hpp"
#include "render/Mesh.hpp"
#include "render/Texture.hpp"
#include "utils/AABB.hpp"

#include <vector>

namespace UI
{
    static const Input* input;

    static u32 target_width;
    static u32 target_height;

    static float    scale;
    static float    offset_x;
    static float    offset_y;

    static Mesh    rect_mesh;
    static Shader  rect_shader;

    struct  DrawInfo
    {
        vec2i   pos;
        vec2i   size;
    };

    static std::vector<DrawInfo>  draws;
}

void    UI::setTargetSize(u32 width, u32 height)
{
    UI::target_width = width;
    UI::target_height = height;
}

void    UI::init()
{
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

void    UI::beginFrame(const Input& input)
{
    UI::input = &input;

    float scaleX = static_cast<float>(UI::input->width()) / UI::target_width;
    float scaleY = static_cast<float>(UI::input->height()) / UI::target_height;
    UI::scale = std::min(scaleX, scaleY);
    UI::offset_x = (UI::input->width()  - UI::target_width  * UI::scale) * 0.5f;
    UI::offset_y = (UI::input->height() - UI::target_height * UI::scale) * 0.5f;
}

void    UI::render()
{
    for (UI::DrawInfo &d : UI::draws)
    {
        const vec2i &pos = d.pos;
        const vec2i &size = d.size;

        glDisable(GL_CULL_FACE);
        UI::rect_shader.bind();
        mat4f model = mat4f::translate(vec3f(pos.x(), pos.y(), 0.0f)) * mat4f::scale(vec3f(size.x(), size.y(), 1.0f));
        UI::rect_shader.setMat4("uModel", model);
        UI::rect_shader.setMat4("uProj", mat4f::ortho(0.0f, UI::input->width(), UI::input->height(), 0.0f, -1.0f, 1.0f));
        UI::rect_shader.setInt("uTex", 0);
        UI::rect_mesh.draw();
        glEnable(GL_CULL_FACE);
    }
    draws.clear();
}

bool    UI::button(const std::string& label, vec2i pos, vec2i size)
{
    vec2i spos = vec2i(pos.x() * UI::scale + UI::offset_x, pos.y() * UI::scale + UI::offset_y);
    vec2i ssize = vec2i(size.x() * UI::scale, size.y() * UI::scale);

    UI::draws.push_back({.pos = spos, .size = ssize});

    aabb2i  box = {.min = spos, .max = spos + ssize};
    if (UI::input->wasPressed(SDL_BUTTON_LEFT)
        && aabb2i::contains(box, vec2i(UI::input->mouseX(), UI::input->mouseY())))
        return (true);
    return (false);
}
