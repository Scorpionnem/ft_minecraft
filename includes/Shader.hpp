/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:53:11 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 14:00:28 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

# include "libs.hpp"

class	Shader
{
	public:
		Shader() {}
		~Shader() {}

		void	load(const std::string &vertexPath, const std::string &fragmentPath);
		void	unload();

		void	bind() const;
		void	setBool(const std::string &name, bool value) const;
		void	setInt(const std::string &name, int value) const;
		void	setFloat(const std::string &name, float value) const;
		void	setMat4(const std::string &name, glm::mat4 value) const;
		void	setVec3(const std::string &name, glm::vec3 value) const;
	private:
		uint	_id;
};

#endif
