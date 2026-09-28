#pragma once

#include "mbl.hpp"
#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>

class	BlockModel
{
	public:
		struct Vertex
		{
			vec3f	pos;
			vec3f	normal;
			vec3f	color;
			vec2f	uv;
		};
		struct Face
		{
			std::vector<BlockModel::Vertex>	vertices;

			std::string	texture;

			// culls face when block here
			mbl::utils::FacingCardinal	cull_face = mbl::utils::FacingCardinal::INVALID;
		};
	public:
		void	load(const std::string& path, mbl::render::TextureAtlas* atlas)
		{
			std::ifstream file(path);
			if (!file.is_open())
				throw std::runtime_error("Failed to open model " + path);

			std::string	line;
			std::string	texture;
			u32			line_number = 0;
			vec3f		color;

			std::string			face;

			try
			{
				while (std::getline(file, line))
				{
					line_number++;
					std::istringstream iss(line);
					std::string prefix;
					iss >> prefix;

					if (prefix == "face")
					{
						if (!(iss >> face))
							throw std::runtime_error("invalid face id");
					}
					if (prefix == "cullface")
					{
						std::string	cf;
						if (!(iss >> cf))
							throw std::runtime_error("invalid cf");
						_faces[face].cull_face = _parse_cullface(cf);
					}
					if (prefix == "texture")
					{
						if (!(iss >> texture))
							throw std::runtime_error("invalid texture");
						if (atlas)
							atlas->add_texture(texture);
					}
					if (prefix == "color")
					{
						float	r, g, b;
						if (!(iss >> r >> g >> b))
							throw std::runtime_error("invalid cf");
						color = vec3f(r, g, b);
					}
					if (prefix == "v")
					{
						float	px, py, pz;
						float	nx, ny, nz;
						float	u, v;
						if (!(iss >> px >> py >> pz >> nx >> ny >> nz >> u >> v))
							throw std::runtime_error("invalid vertex");
						_faces[face].texture = texture;
						_faces[face].vertices.push_back(
							Vertex{
								.pos = vec3f(px, py, pz),
								.normal = vec3f(nx, ny, nz),
								.color = color,
								.uv = vec2f(u, v),
							});
					}
				}
			}
			catch (const std::exception &e)
			{
				throw std::runtime_error(path + " " + std::string(e.what()) + " - line " + std::to_string(line_number));
			}
		}
		void	computeTextures(mbl::render::TextureAtlas* atlas)
		{
			for (auto& [id, f] : _faces)
			{
				for (auto& v : f.vertices)
				{
					if (atlas)
					{
						vec4f uv = atlas->uv(f.texture);
						v.uv.x() = lerp(uv.x(), uv.z(), v.uv.x());
						v.uv.y() = lerp(uv.y(), uv.w(), v.uv.y());
					}
				}
			}
		}
		std::map<std::string, Face>	_faces;
	private:
		mbl::utils::FacingCardinal	_parse_cullface(std::string cf)
		{
			if (cf == "up")
				return (mbl::utils::FacingCardinal::UP);
			if (cf == "down")
				return (mbl::utils::FacingCardinal::DOWN);
			if (cf == "north")
				return (mbl::utils::FacingCardinal::NORTH);
			if (cf == "south")
				return (mbl::utils::FacingCardinal::SOUTH);
			if (cf == "east")
				return (mbl::utils::FacingCardinal::EAST);
			if (cf == "west")
				return (mbl::utils::FacingCardinal::WEST);
			return (mbl::utils::FacingCardinal::INVALID);
		}
};
