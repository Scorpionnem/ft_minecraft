#pragma once

#include "app/scene/Scene.hpp"

class SingleplayerScene: public Scene
{
	public:
		SingleplayerScene() = default;
		~SingleplayerScene() = default;

		void			init() override;
		SceneCommand	update(Client & client, const mbl::platform::Input& input) override;
		void			render() override;
		void			unload() override;

};
