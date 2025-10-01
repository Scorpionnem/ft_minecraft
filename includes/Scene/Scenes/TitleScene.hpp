/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TitleScene.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:11:46 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/01 13:28:41 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TITLESCENE_HPP
# define TITLESCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"

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
		UIElement	*_settingsButton;
		UIElement	*_quitButton;
		UIElement	*_soloButton;
		UIElement	*_multiplayerButton;
		UIElement	*_icon;
		UIElement	*_textTest;
		UIElement	*_textTest1;
		UIElement	*_textTest2;
		UIElement	*_textTest3;
		UIElement	*_textAuthor;
		UIElement	*_fpsText;
		UIElement	*_splashText;
};

#endif
