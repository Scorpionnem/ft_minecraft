/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:16:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 22:09:07 by mbatty           ###   ########.fr       */
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

		bool	isKeyRepeating(int key) const;

		void	setScroll(double value) {_scroll = value;}
		double	getScroll() {return (_scroll);}

		void	reset()
		{
			resetCharInputs();
			resetKeyRepeats();
			setScroll(0);
		}

		void	resetCharInputs()
		{
			_charInputs.clear();
		}
		void	addCharInput(uint c)
		{
			_charInputs.push_back(c);
		}
		std::vector<uint>	getCharInputs()
		{
			return (_charInputs);
		}
		void	setKeyRepeating(int key)
		{
			_repeats[key] = true;
		}
		void	resetKeyRepeats()
		{
			_repeats.fill(0);
		}
	private:
		double							_scroll = 0;
		std::array<bool, MAX_KEYS>	_currentMouse;
		std::array<bool, MAX_KEYS>	_previousMouse;
		std::array<bool, MAX_KEYS>	_current;
		std::array<bool, MAX_KEYS>	_previous;

		std::array<bool, MAX_KEYS>	_repeats;

		std::vector<uint>			_charInputs;
};

#endif
