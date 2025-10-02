/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:11:46 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 12:30:57 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TITLESCENE_HPP
# define TITLESCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"

# include "Panel.hpp"

class TitleScene : public Scene
{
	public:
		TitleScene(Game *game) : Scene(game) {}
		~TitleScene() {};
		
		void onEnter();

		void processInput(float deltaTime);
		void update(float deltaTime);
		void render();

		void onExit();

	private:
		double		_lastSplashTextUpdate = 0;
		Panel		_panel;
};

#endif
