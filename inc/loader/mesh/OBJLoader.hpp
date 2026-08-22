#pragma once

#include "render/Mesh.hpp"
#include "render/TextureAtlas.hpp"
#include <string>

namespace	OBJLoader
{
	struct	Vertex
	{
		vec3f	pos;
		vec3f	normal;
		vec2f	uv;
	};

	void	load(const std::string& path, Mesh& mesh, TextureAtlas &atlas);
};
