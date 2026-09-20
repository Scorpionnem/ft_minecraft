#pragma once

#include "mbl.hpp"

class Client;
struct SceneCommand;

class	Scene
{
	public:
		virtual ~Scene() = default;
		virtual void			init(Client& client) = 0;
		virtual SceneCommand	update(Client & client, const mbl::platform::Input& input) = 0;
		virtual void			render(Client& client) = 0;
		virtual void			unload(Client& client) = 0;
};
