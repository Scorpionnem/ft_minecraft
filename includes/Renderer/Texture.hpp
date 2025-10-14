/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:31:14 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/14 09:57:26 by mbatty           ###   ########.fr       */
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
		void	upload();

		uint	id() {return (this->_id);}
		int		getWidth() {return (this->_width);}
		int		getHeight() {return (this->_height);}
		const std::vector<unsigned char> &getPixels() {return (_pixels);}
		GLenum	getFormat() {return (_format);}
		bool	uploaded() {return (_uploaded);}
	private:
		void						_missingTexture();
		
		int							_width = 0;
		int							_height = 0;
		std::vector<unsigned char>	_pixels;
		GLenum						_format;
		
		bool						_uploaded = false;

		uint						_id = 0;
};

#endif
