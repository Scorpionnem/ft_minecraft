#include "ui/UI.hpp"

const Input* UI::input_ptr;

u32 UI::target_width;
u32 UI::target_height;
u32 UI::target_font_scale = 1;

float    UI::scale;
float    UI::offset_x;
float    UI::offset_y;

Mesh    UI::rect_mesh;
Shader  UI::rect_shader;
Shader  UI::text_shader;
Font	UI::font;

std::vector<UI::DrawInfo>		UI::draws;
std::vector<UI::TextDrawInfo>	UI::text_draws;

std::string	UI::focused_text_input;
std::string	UI::dragging_slider;

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
	UI::text_shader.load("assets/shaders/ui/text.vert", "assets/shaders/ui/text.frag");
}

vec2i	UI::anchorOrigin(vec2i ssize, UI::Anchor anchor)
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

float	UI::getScale()
{
	float scaleX = static_cast<float>(UI::input_ptr->width()) / UI::target_width;
	float scaleY = static_cast<float>(UI::input_ptr->height()) / UI::target_height;

	return (std::max(1.0f, std::round(std::min(scaleX, scaleY))));
}

void    UI::beginFrame(const Input& input)
{
    UI::input_ptr = &input;

    UI::scale = getScale();
    UI::offset_x = (UI::input_ptr->width()  - UI::target_width  * UI::scale) * 0.5f;
    UI::offset_y = (UI::input_ptr->height() - UI::target_height * UI::scale) * 0.5f;
}

void    UI::render()
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (UI::DrawInfo &d : UI::draws)
    {
        const vec2i &pos = d.pos;
        const vec2i &size = d.size;

        rect_shader.bind();
        mat4f model = mat4f::translate(vec3f(pos.x(), pos.y(), 0.0f)) * mat4f::scale(vec3f(size.x(), size.y(), 1.0f));
        rect_shader.setMat4("uModel", model);
        rect_shader.setMat4("uProj", mat4f::ortho(0.0f, UI::input_ptr->width(), UI::input_ptr->height(), 0.0f, -1.0f, 1.0f));
        rect_shader.setInt("uTex", 0);
        rect_shader.setInt("uUseTex", d.textured ? 1 : 0);
        rect_shader.setVec3("uColor", d.text_color);
        rect_shader.setInt("uBackground", d.text_background);
        rect_shader.setVec3("uBackgroundColor", d.text_background_color);

        if (d.textured)
        {
            UI::font.get_atlas().bind(0);
            rect_shader.setVec4("uUV", d.uv);
        }
        else
        {
            rect_shader.setVec3("uColor", d.hovered ? vec3f(0, 1, 0) : vec3f(1, 0, 0));
        }

        UI::rect_mesh.draw(GL_TRIANGLES);
    }
    for (UI::TextDrawInfo& d : UI::text_draws)
    {
	    const vec2i &pos = d.pos;
	    const vec2i &size = d.size;

	    text_shader.bind();
	    mat4f model = mat4f::translate(vec3f(pos.x(), pos.y(), 0.0f)) * mat4f::scale(vec3f(size.x(), size.y(), 1.0f));
	    text_shader.setMat4("uModel", model);
	    text_shader.setMat4("uProj", mat4f::ortho(0.0f, UI::input_ptr->width(), UI::input_ptr->height(), 0.0f, -1.0f, 1.0f));
	    text_shader.setInt("uTex", 0);
	    text_shader.setVec3("uColor", d.color);
	    text_shader.setInt("uBackground", d.background);
	    text_shader.setVec3("uBackgroundColor", d.background_color);
        UI::font.get_atlas().bind(0);
        text_shader.setVec4("uUV", d.uv);

	    UI::rect_mesh.draw(GL_TRIANGLES);
    }

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    draws.clear();
    text_draws.clear();
}

u32	UI::getFontSizeY()
{
	return (UI::font.get_char_size() * UI::target_font_scale);
}

u32	UI::getFontSizeX(char c)
{
	return (UI::font.get_width(c) * UI::target_font_scale);
}

u32	UI::getFontSizeX(const std::string &s)
{
	return (UI::font.get_width(s) * UI::target_font_scale);
}

void	UI::centeredScaledText(const std::string& label, vec2i pos, vec2i size, UI::Anchor anchor)
{
	vec2i	text_pos;
    text_pos.x() = pos.x() + size.x() / 2 - (UI::font.get_width(label) * (int)UI::target_font_scale) / 2;
    text_pos.y() = pos.y() + size.y() / 2 - (UI::font.get_char_size() * (int)UI::target_font_scale) / 2;
    UI::text(label, text_pos, anchor);
}

void	UI::centeredScaledTextY(const std::string& label, vec2i pos, vec2i size, UI::Anchor anchor)
{
	vec2i	text_pos;
    text_pos.x() = pos.x();
    text_pos.y() = pos.y() + size.y() / 2 - (UI::font.get_char_size() * (int)UI::target_font_scale) / 2;
    UI::text(label, text_pos, anchor);
}

void	UI::scalePosAndSize(vec2i& spos, vec2i& ssize, vec2i pos, vec2i size, UI::Anchor anchor)
{
	ssize = vec2i(size.x() * UI::scale, size.y() * UI::scale);
    vec2i origin = anchorOrigin(ssize, anchor);
    spos = vec2i(pos.x() * UI::scale + origin.x(), pos.y() * UI::scale + origin.y());
}

bool	UI::isOnBox(vec2i spos, vec2i ssize)
{
	aabb2i  box = {.min = spos, .max = spos + ssize};

	return (aabb2i::contains(box, vec2i(UI::input_ptr->mouseX(), UI::input_ptr->mouseY())));
}
