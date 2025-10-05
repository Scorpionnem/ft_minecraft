/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:53:11 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/27 19:35:09 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "libs.hpp"
# include "Shader.hpp"

class Camera
{
	public:
		Camera() {}
		~Camera() {}

		void		update(float deltaTime);
		void		setViewProjectionMatrix(Shader *shader, float fov, float aspectRatio, float farPlane);
		glm::mat4	getViewMatrix();

		float	getSpeed() {return (_speed);}
		float	getYaw() {return (_yaw);}
		float	getPitch() {return (_pitch);}

		glm::vec3	getPos() {return (_pos);}
		glm::vec3	getFront() {return (_front);}
		glm::vec3	getUp() {return (_up);}

		void	setPos(const glm::vec3 &pos) {this->_pos = pos;}
		void	setYaw(float yaw) {this->_yaw = yaw;}
		void	setPitch(float pitch) {this->_pitch = pitch;}
	private:
		glm::vec3	_direction;
		glm::vec3	_pos;
		glm::vec3	_lastPos = glm::vec3(0.0);
		glm::vec3	_up = glm::vec3(0.0, 1.0, 0.0);
		glm::vec3	_front;
		glm::vec3	_right;
		double		_yaw = 0.0;
		double		_pitch = 0.0;
		double		_speed = 0.0;
};

#endif
