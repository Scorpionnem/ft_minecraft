#include "App.hpp"

void    App::init(const std::string& ip)
{
	if (client.connect(ip.c_str(), 6767) == -1)
		throw std::runtime_error("connect " + std::string(strerror(errno)));

	threads.add(std::max((u32)1, std::thread::hardware_concurrency()));

	win.open("ft_minecraft", TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mbl::ui::init("assets/textures/font/ascii.png");
	mbl::ui::setTargetSize(TARGET_WINDOW_WIDTH, TARGET_WINDOW_HEIGHT);
	mbl::ui::setTargetFontScale(1);

	frame_buffer.create(win.width(), win.height());

	skybox_shader.load("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
	mesh_shader.load("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
	post_process_shader.load("assets/shaders/post/screen.vert", "assets/shaders/post/screen.frag");
	clouds_shader.load("assets/shaders/post/clouds.vert", "assets/shaders/post/clouds.frag");
	debug_crosshair_shader.load("assets/shaders/debug_crosshair.vert", "assets/shaders/debug_crosshair.frag");

	genScreenMesh();
	genDebugCrosshair();

	mbl::loader::mesh::obj::load("assets/models/teapot.obj", teapot_mesh, test_texture);
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
        const mbl::platform::Input &input = win.pollEvents();

        if (input.close() || input.isDown(SDLK_ESCAPE))
            break ;

        if (input.resize())
        {
            glViewport(0, 0, win.width(), win.height());
            frame_buffer.resize(win.width(), win.height());
        }

        mbl::ui::beginFrame(input);

        switch (state)
        {
            case State::RUNNING:
            	update_running(input); render_running(); break ;
        }

        mbl::ui::render();

        win.swapBuffers();
    }
}

void	App::render_running()
{
	drawn_vertices = 0;

	frame_buffer.bind();
	mbl::render::FrameBuffer::clear();

	skybox_shader.bind();
	skybox_shader.setMat4("uProj", cam.getProjectionMatrix());
	skybox_shader.setMat4("uView", cam.getViewMatrix());
	drawn_vertices += screen_mesh.draw(GL_TRIANGLES);

	mesh_shader.bind();
	mesh_shader.setMat4("uProj", cam.getProjectionMatrix());
	mesh_shader.setMat4("uView", cam.getViewMatrix());
	mesh_shader.setMat4("uModel", mat4f::translate(cur_teapot_pos));
	test_texture.bind(0);
	mesh_shader.setInt("uTex", 0);
	drawn_vertices += teapot_mesh.draw(GL_TRIANGLES);

	frame_buffer.unbind();
	frame_buffer.bindColor(0);
	frame_buffer.bindDepth(1);
	post_process_shader.bind();
	post_process_shader.setInt("uColorFrameBuffer", 0);
	post_process_shader.setInt("uDepthFrameBuffer", 1);
	post_process_shader.setFloat("uNear", cam.near);
	post_process_shader.setFloat("uFar", cam.far);
	drawn_vertices += screen_mesh.draw(GL_TRIANGLES);

	mat4f rotView = cam.getViewMatrix();
	rotView(3, 0) = 0;
	rotView(3, 1) = 0;
	rotView(3, 2) = 0;
	mat4f gizmoView = mat4f::translate(vec3f(0,0,-3)) * rotView;

	glDisable(GL_DEPTH_TEST);
	debug_crosshair_shader.bind();
	debug_crosshair_shader.setMat4("uProj", cam.getProjectionMatrix());
	debug_crosshair_shader.setMat4("uView", gizmoView);
	debug_crosshair_shader.setMat4("uModel", mat4f::scale(vec3f(0.2)));
	debug_crosshair_mesh.draw(GL_LINES);
	glEnable(GL_DEPTH_TEST);
}

void	App::update_running(const mbl::platform::Input& input)
{
	mbl::net::Client::Event	event;
	u8					buf[4096];
	u64					size;

	do
	{
		if (client.recv(buf, sizeof(buf), event, size) == -1)
		{
			throw std::runtime_error("recv");
		}
		if (event == mbl::net::Client::Event::DISCONNECT)
		{
			throw std::runtime_error("disconnected");
		}
		if (event == mbl::net::Client::Event::RECV)
		{
		}
	} while (event != mbl::net::Client::Event::NONE);

	// if (rtt_chrono.get() > 1)
	// {
	// 	rtt_chrono.start();
	// 	rtt_send = Packet::getmsts();
	// 	mbl::Packet::RTTRequest	req;
	// 	client.send(&req, sizeof(req));
	// }

	float	lerp_speed = 16 * input.delta();
	cur_teapot_pos = lerp(cur_teapot_pos, target_teapot_pos, lerp_speed);

	cam.aspect = input.aspect();

	updateCamera(input);

	if (input.wasPressed(SDLK_r))
	{
		try {
			post_process_shader.reload();
		} catch (...)
		{
			std::cerr << "Failed to reload shaders" << std::endl;
		}
	}

	std::string	fps_str = std::to_string(static_cast<int>(1.0 / input.delta())) + " fps";
	std::string	pos_str = "XYZ: " + std::to_string(cam.pos.x()) + " / " + std::to_string(cam.pos.y()) + " / " + std::to_string(cam.pos.z());
	std::string	dir_str = "Facing: " + to_string(static_cast<mbl::utils::FacingCardinal>(mbl::utils::facing(cam.front()))) + " (" + to_string(mbl::utils::facing(cam.front())) + ")";
	std::string	triangles_str = "Triangles: " + std::to_string(drawn_vertices / 3);
	std::string	rtt_str = "RTT: " + std::to_string(rtt) + "ms";
	mbl::ui::text(fps_str, vec2i(0, mbl::ui::getFontSizeY() * 0), mbl::ui::Anchor::TOP_LEFT);
	mbl::ui::text(pos_str, vec2i(0, mbl::ui::getFontSizeY() * 1), mbl::ui::Anchor::TOP_LEFT);
	mbl::ui::text(dir_str, vec2i(0, mbl::ui::getFontSizeY() * 2), mbl::ui::Anchor::TOP_LEFT);
	mbl::ui::text(triangles_str, vec2i(0, mbl::ui::getFontSizeY() * 3), mbl::ui::Anchor::TOP_LEFT);
	mbl::ui::text(rtt_str, vec2i(0, mbl::ui::getFontSizeY() * 4), mbl::ui::Anchor::TOP_LEFT);
}

void    App::updateCamera(const mbl::platform::Input& input)
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

void	App::genDebugCrosshair()
{
	vec3f verts[] = {
		{0,0,0}, {1,0,0}, {1,0,0}, {1,0,0},
		{0,0,0}, {0,1,0}, {0,1,0}, {0,1,0},
		{0,0,0}, {0,0,1}, {0,0,1}, {0,0,1},
	};

	debug_crosshair_mesh.set_sizeof_layout(2 * sizeof(vec3f));
	debug_crosshair_mesh.add_vertex_layout(0, 3, GL_FLOAT, 0);
	debug_crosshair_mesh.add_vertex_layout(1, 3, GL_FLOAT, sizeof(vec3f));
	debug_crosshair_mesh.add_vertex_data(reinterpret_cast<u8*>(verts), sizeof(verts));
	debug_crosshair_mesh.upload();
}
