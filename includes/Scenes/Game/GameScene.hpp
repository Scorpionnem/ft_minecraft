/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameScene.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:01:55 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/07 20:36:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMESCENE_HPP
# define GAMESCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"
# include "Panel.hpp"
# include "WorldGenerator.hpp"

class GameScene : public Scene
{
	public:
		GameScene(Game *game) : Scene(game)
		{
			_id = "GameScene";
		}
		~GameScene() {};

		void onEnter();

		void processInput(float deltaTime);
		void update(UIEvent events, float deltaTime);
		void render();

		void onExit();

	private:
		void	_generateMap();
		Panel	_panel;
		Panel	_mapPanel;
		WorldGenerator	_generator;
};

#endif
