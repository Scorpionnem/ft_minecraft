#pragma once

#include "platform/Window.hpp"
#include "render/Camera.hpp"
#include "render/Mesh.hpp"
#include "utils/Chrono.hpp"

#include <algorithm>

class   App
{
    public:
        void    run()
        {
            init();
            loop();
        }
    private:
        void    init()
        {
            win.open("shaderpixel", 860, 520);

            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            skybox_shader.load("assets/skybox.vert", "assets/skybox.frag");

            vec2f verts[] = {
                {-1.0f, -1.0f},
                { 1.0f, -1.0f},
                { 1.0f,  1.0f},

                { 1.0f,  1.0f},
                {-1.0f,  1.0f},
                {-1.0f, -1.0f},
            };

            screen_mesh.set_sizeof_layout(sizeof(vec2f));
            screen_mesh.add_vertex_layout(0, 2, GL_FLOAT, 0);
            screen_mesh.add_vertex_data(reinterpret_cast<uint8_t*>(verts), sizeof(verts));
            screen_mesh.upload();
        }
        void    loop();
        void    update(const Input &input);
        void    render();
        void    updateCamera(const Input &input);
    private:
        Window  win;
        Camera  cam;

        Chrono  time;

        Shader  skybox_shader;
        Mesh    screen_mesh;
};
