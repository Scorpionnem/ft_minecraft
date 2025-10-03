/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplayerNewScene.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:01:48 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/03 16:04:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPLAYERNEWSCENE_HPP
# define MULTIPLAYERNEWSCENE_HPP


# include "Panel.hpp"
# include "Scene.hpp"
# include "UIElement.hpp"

class	MultiplayerNewScene : public Scene
{
	public:
		MultiplayerNewScene(Game *game) : Scene(game)
		{
			_id = "MultiplayerNewScene";
		}
		~MultiplayerNewScene() {};
		
		void onEnter();

		void processInput(float deltaTime);
		void update(float deltaTime);
		void render();

		void onExit();

	private:
		Panel	_panel;
};

#endif
