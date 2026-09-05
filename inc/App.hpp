#pragma once

#include "platform/Window.hpp"
#include "render/Camera.hpp"
#include "render/FrameBuffer.hpp"
#include "render/Mesh.hpp"
#include "render/TextureAtlas.hpp"
#include "utils/Chrono.hpp"
#include "utils/ThreadPool.hpp"
#include "ui/UI.hpp"

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

        void    run()
        {
            init();
            loop();
        }
    private:
        void    init();
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

		u64	drawn_vertices = 0;
};
