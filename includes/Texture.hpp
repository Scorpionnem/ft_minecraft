/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:31:14 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 15:23:40 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP

#include "libs.hpp"

class	Texture
{
	public:
		Texture() {}
		~Texture() {}

		void	load(const std::string &path);
		void	unload();
		void	bind(int offset);
		void	upload(unsigned char *pixels, int width, int height, GLenum format);

		uint	id() {return (this->_id);}
		int	getWidth() {return (this->_width);}
		int	getHeight() {return (this->_height);}
	private:
		void						_missingTexture();
		uint						_id = 0;
		int							_width = 0;
		int							_height = 0;
		int							_channels = 0;
};

#endif
