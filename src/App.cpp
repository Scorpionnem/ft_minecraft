#include "App.hpp"
#include "loader/mesh/OBJLoader.hpp"
#include "loader/texture/STBLoader.hpp"
#include "mat.hpp"
#include "ui/UI.hpp"

void    App::init()
{
	threads.add(std::max((u32)1, std::thread::hardware_concurrency()));

	win.open("shaderpixel", TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);

	UI::init();
	UI::setTargetSize(TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);

	frame_buffer.create(TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);

	skybox_shader.load("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
	mesh_shader.load("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
	screen_shader.load("assets/shaders/screen.vert", "assets/shaders/screen.frag");

	// STBLoader::load("assets/textures/loading_screen.png", loading_texture);
	// loading_texture.upload();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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
	screen_mesh.add_vertex_data(reinterpret_cast<u8*>(verts), sizeof(verts));
	screen_mesh.upload();

	OBJLoader::load("assets/models/teapot.obj", teapot_mesh);
	teapot_mesh.upload();

	cam.fov = 70;
	cam.far = 1000;
	cam.near = 0.01;
	cam.pos = vec3f(0, 2, 8);
}

void    App::loop()
{
    while (1)
    {
        const Input &input = win.pollEvents();

        if (input.close() || input.isDown(SDLK_ESCAPE))
            break ;

        if (input.resize())
            glViewport(0, 0, win.width(), win.height());

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

void	App::update_running(const Input& input)
{
    cam.aspect = input.aspect();
	updateCamera(input);
}

void	App::render_running()
{
	frame_buffer.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox_shader.bind();
	skybox_shader.setMat4("uProj", cam.getProjectionMatrix());
	skybox_shader.setMat4("uView", cam.getViewMatrix());
	screen_mesh.draw();

	mesh_shader.bind();
	mesh_shader.setMat4("uModel", mat4f::identity());
	mesh_shader.setMat4("uView", cam.getViewMatrix());
	mesh_shader.setMat4("uProj", cam.getProjectionMatrix());
	teapot_mesh.draw();

	frame_buffer.unbind();
	glViewport(0, 0, win.width(), win.height());

	screen_shader.bind();
	frame_buffer.bindColor(0);
	frame_buffer.bindDepth(1);
	screen_shader.setInt("uColorFrameBuffer", 0);
	screen_shader.setInt("uDepthFrameBuffer", 1);
	screen_shader.setInt("uScreenWidth", win.width());
	screen_shader.setInt("uScreenHeight", win.height());
	screen_mesh.draw();
}

void    App::update_loading(const Input& input)
{
	if (threads.active_tasks() == 0
	    && UI::button("start", vec2i(TARGET_WINDOW_WIDTH / 2 - 64, TARGET_WINDOW_HEIGHT / 2 - 32), vec2i(128, 64)))
	{
		state = State::RUNNING;
		test_texture.upload();
	}
}

void    App::render_loading()
{
    loading_texture.bind(0);

	glDepthMask(GL_FALSE);
	screen_shader.bind();
	screen_shader.setInt("uColorFrameBuffer", 0);
	screen_mesh.draw();
	glDepthMask(GL_TRUE);
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
    if (input.isDown(SDLK_a))
        cam.pos = cam.pos + vec3f(-1.0) * (vec3f::normalize(vec3f::cross(cam.front(), vec3f(0, 1, 0))) * move_speed);
    if (input.isDown(SDLK_d))
        cam.pos = cam.pos + (vec3f::normalize(vec3f::cross(cam.front(), vec3f(0, 1, 0))) * move_speed);

    if (input.isDown(SDLK_UP))
        cam.pitch += speed;
    if (input.isDown(SDLK_DOWN))
        cam.pitch -= speed;
    if (input.isDown(SDLK_RIGHT))
        cam.yaw += speed;
    if (input.isDown(SDLK_LEFT))
        cam.yaw -= speed;

    cam.pitch = std::clamp(cam.pitch, -89.0f, 89.0f);

    if (cam.yaw > 360)
        cam.yaw = 0;
    if (cam.yaw < 0)
        cam.yaw = 360;
}
