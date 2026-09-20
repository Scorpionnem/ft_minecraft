#pragma once

#include "app/scene/Scene.hpp"

class MainScene: public Scene
{
	public:
		MainScene() = default;
		~MainScene() = default;

		void			init(Client& client) override;
		SceneCommand	update(Client & client, const mbl::platform::Input& input) override;
		void			render(Client& client) override;
		void			unload(Client& client) override;

};
