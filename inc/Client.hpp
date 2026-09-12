#pragma once

#include "mbl.hpp"

class   Client
{
    #define TARGET_WINDOW_WIDTH 860
    #define TARGET_WINDOW_HEIGHT 520

    private:
        enum class State
        {
            RUNNING,
        };
    public:
        ~Client() {mbl::ui::destroy(); threads.stop();}

        void    run(const std::string& ip, int port)
        {
            init(ip, port);
            loop();
        }
    private:
        void    init(const std::string& ip, int port);
        void    loop();

        void    update_running(const mbl::platform::Input& input);
        void    render_running();

        void    updateCamera(const mbl::platform::Input& input);

        void	genScreenMesh();
        void	genDebugCrosshair();
    private:
   		mbl::utils::ThreadPool	threads;

        State   state = State::RUNNING;

        mbl::platform::Window  win;
        mbl::render::Camera  cam;

        mbl::utils::Chrono  time;

        mbl::render::FrameBuffer	frame_buffer;

        mbl::render::Texture	loading_texture;
        mbl::render::TextureAtlas test_texture;

        mbl::render::Shader  skybox_shader;
        mbl::render::Shader  mesh_shader;
        mbl::render::Shader  post_process_shader;
        mbl::render::Shader  clouds_shader;
        mbl::render::Shader	debug_crosshair_shader;

        mbl::render::Mesh    screen_mesh;
        mbl::render::Mesh    debug_crosshair_mesh;
		mbl::render::Mesh	teapot_mesh;
		vec3f	target_teapot_pos;
		vec3f	cur_teapot_pos;

		mbl::net::Client	client;

		u64	drawn_vertices = 0;
};
