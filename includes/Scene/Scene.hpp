/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:03:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/30 14:38:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_HPP
# define SCENE_HPP

# include "libs.hpp"

class Game;

class Scene
{
	public:
		Scene(Game *game)
		{
			_game = game;
		}
		virtual ~Scene() {};
		
		virtual void onEnter() {};

		virtual void processInput(float deltaTime) = 0;
		virtual void update(float deltaTime) = 0;
		virtual void render() = 0;

		virtual void onExit() {};

		bool	swapScene() {return (!_sceneRequest.empty());}
		std::string	getSceneRequest() {return (_sceneRequest);}
	protected:
		void	requestScene(const std::string &scene)
		{
			_sceneRequest = scene;
		}

		std::string	_sceneRequest;
		Game		*_game;
};

#endif
