#include "platform/Window.hpp"
#include "platform/Input.hpp"

#include <SDL2/SDL.h>
#include "render/GL.hpp"

#include <stdexcept>
#include <string>

Window::Window()
{
}

Window::~Window()
{
	close();
}

void    Window::open(const char *title, u32 size)
{
	open(title, size, size);
}

void    Window::open(const char *title, u32 width, u32 height)
{
	if (_window)
		throw std::runtime_error("Window::open: already open");

	_initSDL();
    _setGLAttributes();
	_createWindow(title, width, height);
    _createGLContext();
	_width = width;
	_height = height;

    glViewport(0, 0, _width, _height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void    Window::close()
{
    if (_glContext)
    {
        SDL_GL_DeleteContext(_glContext);
        _glContext = nullptr;
    }
	if (_window)
	{
		SDL_DestroyWindow(_window);
		_window = nullptr;
	}
	if (SDL_WasInit(SDL_INIT_VIDEO))
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

const Input&    Window::pollEvents()
{
    _input._aspect = static_cast<double>(_width) / static_cast<double>(_height);
    _input._delta = _time.get() - lastFrameTime;
    lastFrameTime = _time.get();
	SDL_Event   event;

	_input.beginFrame();
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			_input._close = true;
		else if (event.type == SDL_KEYDOWN && !event.key.repeat)
			_input.press(event.key.keysym.sym);
		else if (event.type == SDL_KEYUP)
			_input.release(event.key.keysym.sym);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			_input.press(event.button.button);
		else if (event.type == SDL_MOUSEBUTTONUP)
			_input.release(event.button.button);
		else if (event.type == SDL_MOUSEMOTION)
			_input.addMouseDelta(static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel));
		else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			_input._resize = true;
			_width = static_cast<u32>(event.window.data1);
			_height = static_cast<u32>(event.window.data2);
		}
		else if (event.type == SDL_TEXTINPUT)
		    _input._textInput += event.text.text;
	}
	SDL_GetMouseState(&_input._mouseX, &_input._mouseY);
	_input._width = _width;
	_input._height = _height;
	return (_input);
}

void    Window::swapBuffers()
{
    SDL_GL_SwapWindow(_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void    Window::captureMouse(bool captured)
{
	SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}

bool    Window::isMouseCaptured() const
{
	return (SDL_GetRelativeMouseMode() == SDL_TRUE);
}

bool    Window::isOpen() const
{
	return (_window != nullptr);
}

u32     Window::width() const
{
	return (_width);
}

u32     Window::height() const
{
	return (_height);
}

void    Window::_initSDL()
{
	if (SDL_WasInit(SDL_INIT_VIDEO))
		return ;
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init: ") + SDL_GetError());
}

void    Window::_createWindow(const char *title, u32 width, u32 height)
{
	int	mx;
	int	my;
	SDL_GetGlobalMouseState(&mx, &my);

	SDL_Point	p = {mx, my};
	int	disp = SDL_GetPointDisplayIndex(&p);
	if (disp < 0)
		disp = 0;

	_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED_DISPLAY(disp), SDL_WINDOWPOS_CENTERED_DISPLAY(disp), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!_window)
	{
		close();
		throw std::runtime_error(std::string("SDL_CreateWindow: ") + SDL_GetError());
	}
}

void    Window::_setGLAttributes()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void    Window::_createGLContext()
{
    _glContext = SDL_GL_CreateContext(_window);
    if (!_glContext)
    {
        close();
        throw std::runtime_error(std::string("SDL_GL_CreateContext: ") + SDL_GetError());
    }
    SDL_GL_SetSwapInterval(1);
}
