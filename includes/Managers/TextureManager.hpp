/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:47:28 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 11:00:58 by mbatty           ###   ########.fr       */
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

		Texture	*get(const std::string &path)
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_load(path);

			return (&_textures[path]);
		}
		void	upload()
		{
			std::lock_guard<std::mutex> lock(_mutex);
			
			for (auto &texture : _textures)
				if (!texture.second.uploaded())
					texture.second.upload();
		}
	private:
		void	_load(const std::string &path)
		{
			if (_textures.find(path) != _textures.end())
				return ;

			Texture	texture;

			texture.load(path);
			_textures.insert({path, texture});
		}
		std::map<std::string, Texture>	_textures;
		std::mutex						_mutex;
};

#endif
