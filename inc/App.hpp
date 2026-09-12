#pragma once

#include "mbl/inc/mbl.hpp"

using mbl::platform::Input;
using mbl::platform::Window;
using mbl::render::Camera;
using mbl::render::FrameBuffer;
using mbl::render::Texture;
using mbl::render::TextureAtlas;
using mbl::render::Shader;
using mbl::render::Mesh;
using mbl::utils::Chrono;
using mbl::utils::ThreadPool;
using UI = mbl::ui;
namespace net = mbl::net;

class   App
{
    #define TARGET_WINDOW_WIDTH 860
    #define TARGET_WINDOW_HEIGHT 520

    private:
        enum class State
        {
            RUNNING,
        };
    public:
        ~App() {UI::destroy(); threads.stop();}

        void    run(const std::string& ip)
        {
            init(ip);
            loop();
        }
    private:
        void    init(const std::string& ip);
        void    loop();

        void    update_running(const Input& input);
        void    render_running();

        void    updateCamera(const Input& input);

        void	genScreenMesh();
        void	genDebugCrosshair();
    private:
   		ThreadPool	threads;

        State   state = State::RUNNING;

        Window  win;
        Camera  cam;

        Chrono  time;

        FrameBuffer	frame_buffer;

        Texture	loading_texture;
        TextureAtlas test_texture;

        Shader  skybox_shader;
        Shader  mesh_shader;
        Shader  post_process_shader;
        Shader  clouds_shader;
        Shader	debug_crosshair_shader;

        Mesh    screen_mesh;
        Mesh    debug_crosshair_mesh;
		Mesh	teapot_mesh;
		vec3f	target_teapot_pos;
		vec3f	cur_teapot_pos;

		Chrono	rtt_chrono;
		u64	rtt_send = 0;
		u64	rtt = 0;

		net::Client	client;

		u64	drawn_vertices = 0;
};
