#pragma once

#include "math.hpp"
#include <unordered_map>

class Window;

class Input
{
	public:
		Input();

		bool	isDown(int action) const;
		bool	wasPressed(int action) const;
		float	mouseDX() const;
		float	mouseDY() const;
		int		mouseX() const;
		int		mouseY() const;
		bool	close() const;
		bool	resize() const;
        double  delta() const;
        double  aspect() const;
        u32     width() const;
        u32     height() const;

        const std::string   &textInput() const {return (_textInput);};

	private:
		void	beginFrame();
		void	press(int action);
		void	release(int action);
		void	addMouseDelta(float dx, float dy);

		mutable std::unordered_map<int, bool>	_down;
		mutable std::unordered_map<int, bool>	_pressed;
		float	_mouseDX;
		float	_mouseDY;
		int		_mouseX;
		int		_mouseY;

        double  _delta;
        double  _aspect;

        u32 _width;
        u32 _height;

        bool    _close;
		bool	_resize;

		std::string _textInput;

		friend Window;
};
