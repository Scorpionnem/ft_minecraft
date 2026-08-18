#pragma once

#include "platform/Window.hpp"
#include "render/Camera.hpp"
#include "render/Shader.hpp"
#include "utils/Chrono.hpp"

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

    			glBindVertexArray(vao);
                test_shader.bind();
                test_shader.setFloat("time", time.get());
                glDrawArrays(GL_TRIANGLES, 0, 3);
    			glBindVertexArray(0);

                win.swapBuffers();
            }

            glDeleteVertexArrays(1, &vao);
        }
    private:
        Window  win;
        Camera  cam;

        Chrono  time;

        Shader  test_shader;
};
