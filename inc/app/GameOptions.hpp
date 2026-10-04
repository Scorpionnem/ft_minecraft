#pragma once

#include "mbl.hpp"

struct GameOptions
{
    // video
    int     render_distance = 12;
    bool    vsync = true;
    int     max_fps = 0;
    bool    fullscreen = false;
    int     fov = 70;
    // controls
    bool    invert_y = false;
    int     mouse_sensitivity = 100;
    // audio
    // debug
    bool    show_fps = false;
    bool    show_chunk_borders = false;
    bool    show_coords = false;

    enum class Page { VIDEO, CONTROLS, AUDIO, DEBUG };
    GameOptions::Page	page = GameOptions::Page::VIDEO;
};

inline bool	update_options(GameOptions &opts)
{
    mbl::ui::text("Options", vec2f(0, 4), vec2f(0.5, 0));

    vec2i   btn_size = vec2i(100, 20);
    if (mbl::ui::button(opts.page == GameOptions::Page::VIDEO ? "[Video]" : "Video", vec2i(-(btn_size.x() * 1.5 + 6), 8 + mbl::ui::getFontSizeY()), btn_size, vec2f(0.5, 0)))
        opts.page = GameOptions::Page::VIDEO;
    if (mbl::ui::button(opts.page == GameOptions::Page::CONTROLS ? "[Controls]" : "Controls", vec2i(-(btn_size.x() * 0.5 + 2), 8 + mbl::ui::getFontSizeY()), btn_size, vec2f(0.5, 0)))
        opts.page = GameOptions::Page::CONTROLS;
    if (mbl::ui::button(opts.page == GameOptions::Page::AUDIO ? "[Audio]" : "Audio", vec2i((btn_size.x() * 0.5 + 2), 8 + mbl::ui::getFontSizeY()), btn_size, vec2f(0.5, 0)))
        opts.page = GameOptions::Page::AUDIO;
    if (mbl::ui::button(opts.page == GameOptions::Page::DEBUG ? "[Debug]" : "Debug", vec2i((btn_size.x() * 1.5 + 6), 8 + mbl::ui::getFontSizeY()), btn_size, vec2f(0.5, 0)))
        opts.page = GameOptions::Page::DEBUG;

    btn_size = vec2i(150, 20);
    if (opts.page == GameOptions::Page::VIDEO)
    {
        mbl::ui::slider("render_distance_slider", opts.render_distance, 2, 32, vec2i(-(btn_size.x() / 2 + 2), -(btn_size.y() / 2 + 2)), btn_size, ANCHOR_CENTER);
        std::string render_dist_text = "Render Distance: " + std::to_string(opts.render_distance);
        mbl::ui::text(render_dist_text, vec2i(-(btn_size.x() / 2 + 4), -(btn_size.y() / 2 + 2)));

        mbl::ui::slider("fov_slider", opts.fov, 30, 110, vec2i((btn_size.x() / 2 + 2), -(btn_size.y() / 2 + 2)), btn_size, ANCHOR_CENTER);
        std::string fov_text = "FOV: " + (opts.fov == 70 ? "Normal" : opts.fov == 110 ? "Quake Pro" : std::to_string(opts.fov));
        mbl::ui::text(fov_text, vec2i((btn_size.x() / 2 + 4), -(btn_size.y() / 2 + 2)));

        if (mbl::ui::button(opts.fullscreen ? "Fullscreen: ON" : "Fullscreen: OFF", vec2i(-(btn_size.x() / 2 + 2), (btn_size.y() / 2 + 2)), btn_size, ANCHOR_CENTER))
            opts.fullscreen = !opts.fullscreen;
        if (mbl::ui::button(opts.vsync ? "VSync: ON" : "VSync: OFF", vec2i((btn_size.x() / 2 + 2), (btn_size.y() / 2 + 2)), btn_size, ANCHOR_CENTER))
            opts.vsync = !opts.vsync;
    }
    else if (opts.page == GameOptions::Page::CONTROLS)
    {
        mbl::ui::slider("sensivity_slider", opts.mouse_sensitivity, 1, 300, vec2i(-(btn_size.x() / 2 + 2), 0), btn_size, ANCHOR_CENTER);
        std::string sensivity_text = "Sensivity: " + std::to_string(opts.mouse_sensitivity) + "%";
        mbl::ui::text(sensivity_text, vec2i(-(btn_size.x() / 2 + 4), 0));

        if (mbl::ui::button(opts.invert_y ? "Invert Y: ON" : "Invert Y: OFF", vec2i((btn_size.x() / 2 + 2), 0), btn_size, ANCHOR_CENTER))
            opts.invert_y = !opts.invert_y;
    }
    else if (opts.page == GameOptions::Page::AUDIO)
    {
        mbl::ui::text("!!! nah not yet !!!", 0, ANCHOR_CENTER, vec3f(1, 1, 0), true, vec3f(1, 0, 0));
    }
    else if (opts.page == GameOptions::Page::DEBUG)
    {
        if (mbl::ui::button(opts.show_fps ? "Show FPS: ON" : "Show FPS: OFF", vec2i(-(btn_size.x() / 2 + 2), -(btn_size.y() / 2 + 2)), btn_size, ANCHOR_CENTER))
            opts.show_fps = !opts.show_fps;
        if (mbl::ui::button(opts.show_coords ? "Show Coords: ON" : "Show Coords: OFF", vec2i((btn_size.x() / 2 + 2), -(btn_size.y() / 2 + 2)), btn_size, ANCHOR_CENTER))
            opts.show_coords = !opts.show_coords;
        if (mbl::ui::button(opts.show_chunk_borders ? "Chunk Borders: ON" : "Chunk Borders: OFF", vec2i(0, (btn_size.y() / 2 + 2)), btn_size, ANCHOR_CENTER))
            opts.show_chunk_borders = !opts.show_chunk_borders;
    }

    if (mbl::ui::button("Done", vec2f(0, -8.0), vec2f(200, 20), vec2f(0.5, 1.0)))
	    return (false);
    return (true);
}
