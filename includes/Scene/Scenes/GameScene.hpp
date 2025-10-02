/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScene.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:55 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 10:37:40 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMESCENE_HPP
# define GAMESCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"

class GameScene : public Scene
{
	public:
		GameScene(Game *game) : Scene(game) {}
		~GameScene() {};
		
		void onEnter();

		void processInput(float deltaTime);
		void update(float deltaTime);
		void render();

		void onExit();

	private:
};

#endif
