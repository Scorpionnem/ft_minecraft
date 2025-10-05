/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleplayerScene.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:44:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/04 18:36:50 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SINGLEPLAYERSCENE_HPP
# define SINGLEPLAYERSCENE_HPP

# include "Panel.hpp"
# include "Scene.hpp"
# include "UIElement.hpp"

class SingleplayerScene : public Scene
{
	public:
		SingleplayerScene(Game *game) : Scene(game)
		{
			_id = "SingleplayerScene";
			_keepAlive = true;
		}
		~SingleplayerScene() {};

		void onEnter();

		void processInput(float deltaTime);
		void update(UIEvent events, float deltaTime);
		void render();

		void onExit();
	private:
		Panel		_panel;
		Panel		_worldsPanel;
};

#endif
