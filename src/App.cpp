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
	frame_buffer.bind();

	skybox_shader.bind();
	skybox_shader.setFloat("uTime", time.get());
	skybox_shader.setMat4("uProj", cam.getProjectionMatrix());
	skybox_shader.setMat4("uView", cam.getViewMatrix());
	screen_mesh.draw();

	frame_buffer.unbind();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, frame_buffer.id());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, clouds_buffer.id());
	glBlitFramebuffer(0, 0, win.width(), win.height(), 0, 0, win.width(), win.height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	clouds_buffer.bind();

	frame_buffer.bindColor(0);

	clouds_shader.bind();
	clouds_shader.setFloat("uTime", time.get());
	clouds_shader.setInt("uFrameBuffer", 0);
	clouds_shader.setMat4("uProj", cam.getProjectionMatrix());
	clouds_shader.setMat4("uView", cam.getViewMatrix());
	clouds_shader.setVec3("uBoundsMin", vec3f(-32, 0, -32));
	clouds_shader.setVec3("uBoundsMax", vec3f(32, 64, 32));
	screen_mesh.draw();

	clouds_buffer.unbind();

	clouds_buffer.bindColor(0);

	glDepthMask(GL_FALSE);
	screen_shader.bind();
	screen_shader.setInt("uFrameBuffer", 0);
	screen_mesh.draw();
	glDepthMask(GL_TRUE);

	mesh_shader.bind();
	mesh_shader.setMat4("uProj", cam.getProjectionMatrix());
	mesh_shader.setMat4("uView", cam.getViewMatrix());
	mesh_shader.setMat4("uModel", mat4f::scale(vec3f(4)) * mat4f::translate(vec3f(-5, 5, -5)));
	teapot_mesh.draw();
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
