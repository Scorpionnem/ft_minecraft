#pragma once

#include "math.hpp"
#include "platform/Input.hpp"
#include "render/GL.hpp"

#include <SDL2/SDL.h>

#include <string>

class Window
{
	public:
		Window();
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		void    open(const char* title, u32 width, u32 height);
		void    open(const char* title, u32 size);
		void    close();

		const Input&    pollEvents();

		void    swapBuffers();

		void    captureMouse(bool captured);
		bool    isMouseCaptured() const;

		bool    isOpen() const;
		u32     width() const;
		u32     height() const;

	private:
		void    _initSDL();
		void    _createWindow(const char* title, u32 width, u32 height);
        void    _setGLAttributes();
        void    _createGLContext();

        Input       _input;

		SDL_Window* _window = nullptr;
        void*       _glContext = nullptr;

		u32         _width = 0;
		u32         _height = 0;
};
