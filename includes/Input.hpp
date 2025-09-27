/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:16:33 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 13:19:41 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HPP
# define INPUT_HPP

# include "libs.hpp"
# define MAX_KEYS 512

class Input
{
	public:
		void	update(GLFWwindow *window);
		bool	isKeyDown(int key) const;
		bool	isKeyPressed(int key) const;
		bool	isKeyReleased(int key) const;

	private:
		std::array<bool, MAX_KEYS>	_current;
		std::array<bool, MAX_KEYS>	_previous;
};

#endif
