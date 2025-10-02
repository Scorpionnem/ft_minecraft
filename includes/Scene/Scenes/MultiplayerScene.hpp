/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplayerScene.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:44:26 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 11:44:57 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIPLAYERSCENE_HPP
# define MULTIPLAYERSCENE_HPP

# include "Panel.hpp"
# include "Scene.hpp"
# include "UIElement.hpp"

class MultiplayerScene : public Scene
{
	public:
		MultiplayerScene(Game *game) : Scene(game) {}
		~MultiplayerScene() {};
		
		void onEnter();

		void processInput(float deltaTime);
		void update(float deltaTime);
		void render();

		void onExit();

	private:
		Panel	_panel;
};

#endif
