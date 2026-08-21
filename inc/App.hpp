#pragma once

#include "platform/Window.hpp"
#include "render/Camera.hpp"
#include "render/Mesh.hpp"
#include "render/Texture.hpp"
#include "render/TextureAtlas.hpp"
#include "render/FrameBuffer.hpp"
#include "utils/Chrono.hpp"
#include "loader/mesh/OBJLoader.hpp"
#include "loader/texture/STBLoader.hpp"

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

			skybox_shader.load("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");

			clouds_shader.load("assets/shaders/clouds.vert", "assets/shaders/clouds.frag");

			mesh_shader.load("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");

			screen_shader.load("assets/shaders/screen.vert", "assets/shaders/screen.frag");


			test_texture.add_texture("assets/textures/test1.png");
			test_texture.add_texture("assets/textures/test2.png");
			test_texture.add_texture("assets/textures/test3.png");
			test_texture.add_texture("assets/textures/test6.png");
			test_texture.upload();

			frame_buffer.create(win.width(), win.height(), false);
			clouds_buffer.create(win.width(), win.height(), false);

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

			OBJLoader::load("assets/models/teapot.obj", teapot_mesh);
			teapot_mesh.upload();

			cam.fov = 70;
			cam.far = 1000;
			cam.near = 0.01;
			cam.pos = vec3f(0, 2, 8);
        }
        void    loop();
        void    update(const Input &input);
        void    render();
        void    updateCamera(const Input &input);
    private:
        Window  win;
        Camera  cam;

		FrameBuffer	frame_buffer;
		FrameBuffer	clouds_buffer;

        Chrono  time;

        Shader  clouds_shader;

        TextureAtlas test_texture;

        Shader  skybox_shader;
        Mesh    screen_mesh;
        Shader  screen_shader;

        Shader  mesh_shader;
		Mesh	teapot_mesh;
};
