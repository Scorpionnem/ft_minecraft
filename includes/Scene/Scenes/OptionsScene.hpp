/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OptionsScene.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:11:46 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/02 11:51:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONSSCENE_HPP
# define OPTIONSSCENE_HPP

# include "Panel.hpp"
# include "Scene.hpp"
# include "UIElement.hpp"

class OptionsScene : public Scene
{
	public:
		OptionsScene(Game *game) : Scene(game) {}
		~OptionsScene() {};
		
		void onEnter();

		void processInput(float deltaTime);
		void update(float deltaTime);
		void render();

		void onExit();

	private:
		Panel	_panel;
};

#endif
