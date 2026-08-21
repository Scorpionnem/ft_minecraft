#include "platform/Input.hpp"

Input::Input() : _mouseDX(0.0f), _mouseDY(0.0f)
{
	_pressed.clear();
	_down.clear();
}

void    Input::beginFrame()
{
	_resize = false;
	_close = false;
	_pressed.clear();
    _mouseDX = 0.0f;
    _mouseDY = 0.0f;
    _textInput.clear();
}

void    Input::press(int action)
{
    _down[action] = true;
    _pressed[action] = true;
}

void    Input::release(int action)
{
    _down[action] = false;
}

void    Input::addMouseDelta(float dx, float dy)
{
    _mouseDX += dx;
    _mouseDY += dy;
}

bool    Input::isDown(int action) const
{
    return (_down[action]);
}

bool    Input::wasPressed(int action) const
{
    return (_pressed[action]);
}

float   Input::mouseDX() const
{
    return (_mouseDX);
}

float   Input::mouseDY() const
{
    return (_mouseDY);
}

int   Input::mouseX() const
{
    return (_mouseX);
}

int   Input::mouseY() const
{
    return (_mouseY);
}

bool    Input::close() const
{
    return (_close);
}

bool	Input::resize() const
{
	return (_resize);
}

double    Input::delta() const
{
    return (_delta);
}

double    Input::aspect() const
{
    return (_aspect);
}

u32    Input::width() const
{
    return (_width);
}

u32    Input::height() const
{
    return (_height);
}
