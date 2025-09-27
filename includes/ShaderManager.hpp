/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:01:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 14:14:02 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERMANAGER_HPP
# define SHADERMANAGER_HPP

# include "Shader.hpp"

class	ShaderManager
{
	public:
		ShaderManager() {}
		~ShaderManager() {}

		void	load(const std::string &id, const std::string &vertpath, const std::string &fragpath)
		{
			if (_shaders.find(id) != _shaders.end())
				return ;
			_shaders.insert(std::make_pair(id, Shader()));
			_shaders[id].load(vertpath, fragpath);
		}
		Shader	*get(const std::string &id)
		{
			if (_shaders.find(id) == _shaders.end())
				return (NULL);
			
			return (&_shaders[id]);
		}
	private:
		std::map<std::string, Shader>	_shaders;
};

#endif
