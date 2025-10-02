/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:47:28 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 10:46:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTUREMANAGER_HPP
# define TEXTUREMANAGER_HPP

# include "Texture.hpp"

class	TextureManager
{
	public:
		TextureManager() {}
		~TextureManager() {}

		void	load(const std::string &id, const std::string &path)
		{
			if (_textures.find(id) != _textures.end())
				return ;

			_textures.insert({id, Texture()});
			_textures[id].load(path);
		}
		void	load(const std::string &path)
		{
			load(path, path);
		}
		Texture	*get(const std::string &id)
		{
			load(id);
			if (_textures.find(id) == _textures.end())
				return (NULL);
			
			return (&_textures[id]);
		}
	private:
		std::map<std::string, Texture>	_textures;
};

#endif
