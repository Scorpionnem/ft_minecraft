/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:47:28 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 20:04:19 by mbatty           ###   ########.fr       */
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

			Texture	texture;

			texture.load(path);
			_textures.insert({id, texture});
		}
		void	load(const std::string &path)
		{
			load(path, path);
		}
		Texture	*get(const std::string &id)
		{
			if (id != "missing" && _textures.find(id) == _textures.end())
				return (get("missing"));

			return (&_textures[id]);
		}
	private:
		std::map<std::string, Texture>	_textures;
};

#endif
