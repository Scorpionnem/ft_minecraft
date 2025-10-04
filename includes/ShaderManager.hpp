/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:01:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/04 14:50:52 by mbatty           ###   ########.fr       */
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
			
			Shader	shader;

			shader.setValid(true);

			try {
				shader.load(vertpath, fragpath);
			} catch(const std::exception& e) {
				std::cerr << e.what() << '\n';
				shader.setValid(false);
			}

			_shaders.insert(std::make_pair(id, shader));
		}
		void	reload()
		{
			for (auto &shader : _shaders)
			{
				try {
					shader.second.reload();
				} catch(const std::exception& e) {
					std::cerr << e.what() << '\n';
					shader.second.setValid(false);
					continue ;
				}
				shader.second.setValid(true);
			}
		}
		Shader	*get(const std::string &id)
		{
			if (_shaders.find(id) == _shaders.end())
				return (NULL);

			Shader	*ret = &_shaders[id];
			if (!ret->getValid())
				return (NULL);

			return (ret);
		}
	private:
		std::map<std::string, Shader>	_shaders;
};

#endif
