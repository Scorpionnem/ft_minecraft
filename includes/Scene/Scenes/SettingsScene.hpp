/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SettingsScene.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:11:46 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 10:22:30 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGSSCENE_HPP
# define SETTINGSSCENE_HPP

# include "Scene.hpp"
# include "UIElement.hpp"

class SettingsScene : public Scene
{
	public:
		SettingsScene(Game *game) : Scene(game) {}
		~SettingsScene() {};
		
		void onEnter();

		void processInput(float deltaTime);
		void update(float deltaTime);
		void render();

		void onExit();

	private:
};

#endif
