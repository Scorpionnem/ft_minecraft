#include "App.hpp"
#include "SDL_keycode.h"
#include "loader/mesh/OBJLoader.hpp"
#include "loader/texture/STBLoader.hpp"
#include "mat.hpp"
#include "math.hpp"
#include "render/FrameBuffer.hpp"
#include "ui/UI.hpp"
#include <GL/gl.h>
#include <string>

#include "vox/Positions.hpp"

void    App::init()
{
	threads.add(std::max((u32)1, std::thread::hardware_concurrency()));

	win.open("shaderpixel", TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	UI::init("assets/textures/font/ascii.png");
	UI::setTargetSize(TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);
	UI::setTargetFontScale(2);

	frame_buffer.create(win.width(), win.height());

	skybox_shader.load("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
	mesh_shader.load("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
	post_process_shader.load("assets/shaders/post/screen.vert", "assets/shaders/post/screen.frag");
	clouds_shader.load("assets/shaders/post/clouds.vert", "assets/shaders/post/clouds.frag");

	genScreenMesh();

	OBJLoader::load("assets/models/teapot.obj", teapot_mesh, test_texture);
	teapot_mesh.upload();
	test_texture.upload();

	cam.fov = 70;
	cam.near = 0.01;
	cam.far = 1000;
	cam.pos = vec3f(0, 0, 0);
}

void    App::loop()
{
    while (1)
    {
        const Input &input = win.pollEvents();

        if (input.close() || input.isDown(SDLK_ESCAPE))
            break ;

        if (input.resize())
        {
            glViewport(0, 0, win.width(), win.height());
            frame_buffer.resize(win.width(), win.height());
        }

        UI::beginFrame(input);

        switch (state)
        {
            case State::LOADING:
                update_loading(input); render_loading(); break ;
            case State::RUNNING:
            	update_running(input); render_running(); break ;
        }

        UI::render();

        win.swapBuffers();
    }
}

void	App::render_running()
{
	drawn_vertices = 0;

	frame_buffer.bind();
	FrameBuffer::clear();

	skybox_shader.bind();
	skybox_shader.setMat4("uProj", cam.getProjectionMatrix());
	skybox_shader.setMat4("uView", cam.getViewMatrix());
	drawn_vertices += screen_mesh.draw();

	mesh_shader.bind();
	mesh_shader.setMat4("uProj", cam.getProjectionMatrix());
	mesh_shader.setMat4("uView", cam.getViewMatrix());
	mesh_shader.setMat4("uModel", mat4f::identity());
	test_texture.bind(0);
	mesh_shader.setInt("uTex", 0);
	drawn_vertices += teapot_mesh.draw();

	frame_buffer.unbind();
	frame_buffer.bindColor(0);
	frame_buffer.bindDepth(1);
	post_process_shader.bind();
	post_process_shader.setInt("uColorFrameBuffer", 0);
	post_process_shader.setInt("uDepthFrameBuffer", 1);
	post_process_shader.setFloat("uNear", cam.near);
	post_process_shader.setFloat("uFar", cam.far);
	drawn_vertices += screen_mesh.draw();
}

void	App::update_running(const Input& input)
{
	cam.aspect = input.aspect();

	updateCamera(input);

	if (input.wasPressed(SDLK_r))
	{
		try {
		clouds_shader.reload();
		} catch (...)
		{

		}
	}

	std::string	fps_str = std::to_string(static_cast<int>(1.0 / input.delta())) + " fps";
	std::string	pos_str = "XYZ: " + std::to_string(cam.pos.x()) + " / " + std::to_string(cam.pos.y()) + " / " + std::to_string(cam.pos.z());
	std::string	dir_str = "Facing: " + std::to_string(static_cast<FacingCardinal>(facing(cam.front()))) + " (" + std::to_string(facing(cam.front())) + ")";
	std::string	triangles_str = "Triangles: " + std::to_string(drawn_vertices / 3);
	UI::text(fps_str, vec2i(0, UI::getFontSizeY() * 0), UI::Anchor::TOP_LEFT);
	UI::text(pos_str, vec2i(0, UI::getFontSizeY() * 1), UI::Anchor::TOP_LEFT);
	UI::text(dir_str, vec2i(0, UI::getFontSizeY() * 2), UI::Anchor::TOP_LEFT);
	UI::text(triangles_str, vec2i(0, UI::getFontSizeY() * 3), UI::Anchor::TOP_LEFT);
}

void    App::update_loading(const Input& input)
{
	if (UI::button("START", vec2i(TARGET_WINDOW_WIDTH / 2 - 64, TARGET_WINDOW_HEIGHT / 2 - 32), vec2i(128, 64), UI::Anchor::CENTER))
	{
		state = State::RUNNING;
	}
}

void    App::render_loading()
{
}

void    App::updateCamera(const Input& input)
{
    float   move_speed = 20 * input.delta();
    float   speed = 100 * input.delta();

    if (input.isDown(SDLK_w))
        cam.pos = cam.pos + (cam.front() * move_speed);
    if (input.isDown(SDLK_s))
        cam.pos = cam.pos + vec3f(-1.0) * (cam.front() * move_speed);
    if (input.isDown(SDLK_SPACE))
        cam.pos = cam.pos + (vec3f(0, 1, 0) * move_speed);
    if (input.isDown(SDLK_LSHIFT))
        cam.pos = cam.pos + vec3f(-1.0) * (vec3f(0, 1, 0) * move_speed);

    vec3f right = vec3f(cos(radians(cam.yaw)), 0.0f, sin(radians(cam.yaw)));

    if (input.isDown(SDLK_a))
        cam.pos = cam.pos - right * move_speed;
    if (input.isDown(SDLK_d))
        cam.pos = cam.pos + right * move_speed;

    if (input.isDown(SDLK_UP))
        cam.pitch += speed;
    if (input.isDown(SDLK_DOWN))
        cam.pitch -= speed;
    if (input.isDown(SDLK_RIGHT))
        cam.yaw += speed;
    if (input.isDown(SDLK_LEFT))
        cam.yaw -= speed;

    cam.pitch = std::clamp(cam.pitch, -90.0f, 90.0f);

    if (cam.yaw > 360)
        cam.yaw = 0;
    if (cam.yaw < 0)
        cam.yaw = 360;
}

void	App::genScreenMesh()
{
	vec2f verts[] = {
		{-1.0f, -1.0f},
		{ 1.0f, -1.0f},
		{ 1.0f,  1.0f},
	};

	screen_mesh.set_sizeof_layout(sizeof(vec2f));
	screen_mesh.add_vertex_layout(0, 2, GL_FLOAT, 0);
	screen_mesh.add_vertex_data(reinterpret_cast<u8*>(verts), sizeof(verts));
	screen_mesh.upload();
}
