#pragma once

#include "platform/Window.hpp"
#include "render/Camera.hpp"
#include "render/Shader.hpp"
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
            win.open("shaderpixel", 512);
            test_shader.load("assets/test.vert", "assets/test.frag");
        }
        void    loop()
        {
            u32 vao = 0;

            glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glBindVertexArray(0);

            while (1)
            {
                const Input &input = win.pollEvents();

                if (input.close() || input.isDown(SDLK_ESCAPE))
                    break ;

                cam.fov = 70;
                cam.far = 1000;
                cam.near = 0.01;
                cam.aspect = input.aspect();

                update(input);
                render();

    			glBindVertexArray(vao);
                test_shader.bind();
                test_shader.setMat4("uProj", cam.getProjectionMatrix());
                test_shader.setMat4("uView", cam.getViewMatrix());

                glDrawArrays(GL_TRIANGLES, 0, 3);
    			glBindVertexArray(0);

                win.swapBuffers();
            }

            glDeleteVertexArrays(1, &vao);
        }
        void    update(const Input &input)
        {
            if (input.wasPressed(SDLK_r))
                test_shader.reload();
            updateCamera(input);
        }
        void    render()
        {

        }
        void    updateCamera(const Input &input)
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
    private:
        Window  win;
        Camera  cam;

        Chrono  time;

        Shader  test_shader;
};
