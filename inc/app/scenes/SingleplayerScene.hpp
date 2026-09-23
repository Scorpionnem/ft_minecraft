#pragma once

#include "app/scene/Scene.hpp"

class SingleplayerScene: public Scene
{
	public:
		SingleplayerScene() = default;
		~SingleplayerScene() = default;

		void			init(Client& client) override;
		SceneCommand	update(Client & client, const mbl::platform::Input& input) override;
		void			render(Client& client) override;
		void			unload(Client& client) override;
};
