/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SingleplayerScene.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:44:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 11:22:45 by mbatty           ###   ########.fr       */
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
		void update(float deltaTime);
		void render();

		void onExit();
	private:
		uint64_t	_timesClicked = 0;
		Panel		_panel;
};

#endif
