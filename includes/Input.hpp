/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:16:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 21:00:40 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HPP
# define INPUT_HPP

# include "libs.hpp"
# define MAX_KEYS 512
# define MAX_MOUSE_KEYS 12

class Input
{
	public:
		void	update(GLFWwindow *window);
		bool	isKeyDown(int key) const;
		bool	isMouseDown(int key) const;

		bool	isMousePressed(int key) const;
		bool	isMouseReleased(int key) const;
		bool	isKeyPressed(int key) const;
		bool	isKeyReleased(int key) const;

		void	setScroll(double value) {_scroll = value;}
		double	getScroll() {return (_scroll);}
	private:
		double							_scroll = 0;
		std::array<bool, MAX_KEYS>	_currentMouse;
		std::array<bool, MAX_KEYS>	_previousMouse;
		std::array<bool, MAX_KEYS>	_current;
		std::array<bool, MAX_KEYS>	_previous;
};

#endif
