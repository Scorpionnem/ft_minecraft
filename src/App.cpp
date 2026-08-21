#include "App.hpp"

void    App::loop()
{
    while (1)
    {
        const Input &input = win.pollEvents();

        if (input.close() || input.isDown(SDLK_ESCAPE))
            break ;

        update(input);
        render();

        win.swapBuffers();
    }
}

void    App::update(const Input &input)
{
	if (input.resize())
	{
        glViewport(0, 0, win.width(), win.height());
		frame_buffer.resize(win.width(), win.height());
		clouds_buffer.resize(win.width(), win.height());
	}
    cam.aspect = input.aspect();

    if (input.wasPressed(SDLK_r))
	{
        skybox_shader.reload();
		clouds_shader.reload();
	}
    updateCamera(input);
}

void    App::render()
{
    test_texture.bind(0);

	glDepthMask(GL_FALSE);
	screen_shader.bind();
	screen_shader.setInt("uFrameBuffer", 0);
	screen_mesh.draw();
	glDepthMask(GL_TRUE);
}

void    App::updateCamera(const Input &input)
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
