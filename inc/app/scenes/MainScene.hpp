#pragma once

#include "app/scene/Scene.hpp"

class MainScene: public Scene
{
	public:
		MainScene() = default;
		~MainScene() = default;

		void			init() override;
		SceneCommand	update(Client & client, const mbl::platform::Input& input) override;
		void			render() override;
		void			unload() override;

};
