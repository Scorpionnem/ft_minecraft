/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:03:51 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 12:26:55 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_HPP
# define SCENE_HPP

# include "libs.hpp"
# include "UIElement.hpp"

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
		virtual void update(UIEvent events, float deltaTime) = 0;
		virtual void render() = 0;

		virtual void onExit() {};

		void		resetRequest() {_sceneRequest = NULL;}
		bool		requestedScene() {return (_sceneRequest != NULL);}
		Scene		*getSceneRequest() {return (_sceneRequest);}
		
		std::string	id() {return (_id);}
		bool		keepAlive() {return (_keepAlive);}
		bool		loaded() {return (this->_loaded);}
	protected:
		void	_requestScene(Scene *scene)
		{
			_sceneRequest = scene;
		}

		std::atomic<bool>	_loaded = false;
		Scene		*_sceneRequest = NULL;
		Game		*_game;
		std::string	_id;
		bool		_keepAlive = false;
};

#endif
