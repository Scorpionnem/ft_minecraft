/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ImprovedSlider.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:23:22 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/18 10:24:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPROVEDSLIDER_HPP
# define IMPROVEDSLIDER_HPP

# include "UIElement.hpp"
# include "ImprovedButton.hpp"

// ImprovedButton(glm::vec2 size, glm::vec2 anchor, glm::vec2 offset, Shader *shader, Texture *defaultTexture, Texture *hoverTexture, Texture *disabledTexture);

/*

	slider
	slider_highlighted

	slider_handle
	slider_handle_highlighted

*/

/*

	todo

	make float slider, int slider, min val, max val

*/

// template	<typename  T>
class	ImprovedSlider : public UIElement
{
	public:
		ImprovedSlider(glm::vec2 size, glm::vec2 sliderSsize, glm::vec2 anchor, glm::vec2 offset, float startVal, Shader *shader, Texture *slider, Texture *sliderHandle, Texture *sliderHandleHover)
		: _slider(size, anchor, offset, shader, slider, NULL, NULL), _sliderHandle(sliderSsize, anchor, offset, shader, sliderHandle, sliderHandleHover, NULL)
		{
			_val = startVal;
			_setSlider(false);

			_slider.setClickFunc([this]
				()
				{
					_computeSliderVal();
					_setSlider(true);
				});
		}
		~ImprovedSlider() {}

		void	handleEvents(UIEvent events)
		{
			_windowSize = events.windowSize;
			_mousePos = events.mousePos;
			
			_slider.handleEvents(events);
			_sliderHandle.handleEvents(events);
			_sliderHandle.setHovered(_slider.isHovered());

			_slider.setHovered(false);
		}

		void	draw(glm::vec2 windowSize)
		{
			_slider.draw(windowSize);
			_sliderHandle.draw(windowSize);
		}

		void	setVal(float val)
		{
			_val = glm::clamp(val, 0.f, 1.f);
			_setSlider(false);
		}

		void	setClickFunc(std::function<void(float)> func) {_onClick = func;}
	private:
		void			_computeSliderVal()
		{
			float	sliderSize = _slider.getSize().x * UIElement::_getUiScale(_windowSize);
			float	sliderPos = UIElement::_getScaledPos(_slider.getSize(), _slider.getAnchor(), _slider.getOffset(), _windowSize).x;
			
			_val = _mousePos.x / (sliderPos + sliderSize);
		}
		void			_setSlider(bool click)
		{
			float	x = _val * _slider.getSize().x;
			
			x -= (_val * (_sliderHandle.getSize().x / 2)) + ((1 - _val) * (_sliderHandle.getSize().x / 2));

			_sliderHandle.setOffsetX(_slider.getOffset().x + x);
			if (_onClick && click)
				_onClick(_val);
		}
		ImprovedButton	_slider;
		ImprovedButton	_sliderHandle;

		glm::vec2		_windowSize;
		glm::vec2		_mousePos;

		std::function<void(float)>	_onClick = NULL;

		float			_val = 0;
};

#endif
