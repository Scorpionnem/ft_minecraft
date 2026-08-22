#include "loader/mesh/OBJLoader.hpp"

#include <cmath>
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <string>

static void	parse_vert(std::istringstream& iss, std::vector<vec3f>& pos_vec)
{
	float x, y, z;
	if (!(iss >> x >> y >> z))
		throw std::runtime_error("Invalid vertex pos");
	pos_vec.push_back(vec3f(x, y, z));
}

static void	parse_uv(std::istringstream& iss, std::vector<vec2f>& uv_vec)
{
	float x, y;
	if (!(iss >> x >> y))
		throw std::runtime_error("Invalid vertex uv");
	uv_vec.push_back(vec2f(x, y));
}

static vec2f	remap_uv(const vec2f& uv, const vec4f& atlas_rect)
{
	return (vec2f(
		atlas_rect.x() + uv.x() * (atlas_rect.z() - atlas_rect.x()),
		atlas_rect.y() + uv.y() * (atlas_rect.w() - atlas_rect.y())
	));
}

static void	parse_face(Mesh& mesh, std::istringstream& iss, std::vector<vec3f>& pos_vec, std::vector<vec2f>& uv_vec, const vec4f& current_uv_off)
{
	struct FaceVertex
	{
		int pos_index;
		int uv_index;
	};

	std::vector<FaceVertex> face_indices;

	std::string token;
	while (iss >> token)
	{
		std::istringstream ss(token);
		std::string pos_str, uv_str;

		std::getline(ss, pos_str, '/');
		std::getline(ss, uv_str, '/');

		FaceVertex fv = { -1, -1 };

		try {
			fv.pos_index = std::stoi(pos_str) - 1;
			if (!uv_str.empty())
				fv.uv_index = std::stoi(uv_str) - 1;
		}
		catch (...) {
			throw std::runtime_error("stoi");
		}

		face_indices.push_back(fv);
	}

	if (face_indices.size() < 3)
		throw std::runtime_error("less than 3 vertices");

	for (size_t i = 1; i + 1 < face_indices.size(); ++i)
	{
		const FaceVertex &fv1 = face_indices[0];
		const FaceVertex &fv2 = face_indices[i];
		const FaceVertex &fv3 = face_indices[i + 1];

		if (fv1.pos_index < 0 || fv2.pos_index < 0 || fv3.pos_index < 0
			|| fv1.pos_index > (int)pos_vec.size() - 1
			|| fv2.pos_index > (int)pos_vec.size() - 1
			|| fv3.pos_index > (int)pos_vec.size() - 1)
			throw std::runtime_error("Invalid vertice pos index");
		vec3f	pos1 = pos_vec[fv1.pos_index];
		vec3f	pos2 = pos_vec[fv2.pos_index];
		vec3f	pos3 = pos_vec[fv3.pos_index];

		if (fv1.uv_index < -1 || fv2.uv_index < -1 || fv3.uv_index < -1
			|| fv1.uv_index > (int)uv_vec.size() - 1
			|| fv2.uv_index > (int)uv_vec.size() - 1
			|| fv3.uv_index > (int)uv_vec.size() - 1)
			throw std::runtime_error("Invalid vertice uv index");
		vec2f	uv1 = fv1.uv_index == -1 ? vec2f(0) : remap_uv(uv_vec[fv1.uv_index], current_uv_off);
		vec2f	uv2 = fv1.uv_index == -1 ? vec2f(0) : remap_uv(uv_vec[fv2.uv_index], current_uv_off);
		vec2f	uv3 = fv1.uv_index == -1 ? vec2f(0) : remap_uv(uv_vec[fv3.uv_index], current_uv_off);

		OBJLoader::Vertex	triangle[3] =
		{
			{
				.pos = pos1,
				.normal = vec3f(0),
				.uv = uv1,
			},
			{
				.pos = pos2,
				.normal = vec3f(0),
				.uv = uv2,
			},
			{
				.pos = pos3,
				.normal = vec3f(0),
				.uv = uv3,
			}
		};

		mesh.add_vertex_data(reinterpret_cast<u8*>(&triangle), sizeof(triangle));
	}
}

static void	loadMTL(const std::string& obj_path, std::map<std::string, std::string>& mtl_tex_path, TextureAtlas &atlas, std::istringstream& iss)
{
	std::string	mtllib;
	if (!(iss >> mtllib))
		throw std::runtime_error("invalid mtllib");

	std::string	path = std::filesystem::path(obj_path).parent_path().string() + "/" + mtllib;

	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Failed to open model " + path);

	std::string	line;
	std::string	current_mtl;
	u32			line_number = 0;

	try
	{
		while (std::getline(file, line))
		{
			line_number++;
			std::istringstream iss(line);
			std::string prefix;
			iss >> prefix;

			if (prefix == "newmtl")
			{
				if (!(iss >> current_mtl))
					throw std::runtime_error("invalid newmtl");
			}
			if (prefix == "map_Kd")
			{
				std::string	tex_path;
				if (!(iss >> tex_path))
					throw std::runtime_error("invalid map_Kd");

				tex_path = std::filesystem::path(path).parent_path().string() + "/" + tex_path;

				mtl_tex_path[current_mtl] = tex_path;

				if (atlas.has(tex_path))
					continue ;
				atlas.add_texture(tex_path);
			}
		}
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error(path + " " + std::string(e.what()) + " - line " + std::to_string(line_number));
	}
}

void	OBJLoader::load(const std::string& path, Mesh& mesh, TextureAtlas &atlas)
{
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Failed to open model " + path);

	vec2f verts[] = {
			{-1.0f, -1.0f},
			{ 1.0f, -1.0f},
			{ 1.0f,  1.0f},

			{ 1.0f,  1.0f},
			{-1.0f,  1.0f},
			{-1.0f, -1.0f},
		};

	mesh.set_sizeof_layout(sizeof(OBJLoader::Vertex));
	mesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(OBJLoader::Vertex, pos));
	mesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(OBJLoader::Vertex, normal));
	mesh.add_vertex_layout(2, 2, GL_FLOAT, offsetof(OBJLoader::Vertex, uv));

	std::vector<vec3f>	pos_vec;
	std::vector<vec2f>	uv_vec;

	std::map<std::string, std::string>	mtl_tex_path;

	std::string	line;
	u32			line_number = 0;

	vec4f	current_uv_off = vec4f(0, 0, 1, 1);

	try
	{
		while (std::getline(file, line))
		{
			line_number++;
			std::istringstream iss(line);
			std::string prefix;
			iss >> prefix;

			if (prefix == "v")
			{
				parse_vert(iss, pos_vec);
			}
			if (prefix == "vt")
			{
				parse_uv(iss, uv_vec);
			}
			else if (prefix == "f")
			{
				parse_face(mesh, iss, pos_vec, uv_vec, current_uv_off);
			}
			else if (prefix == "mtllib")
			{
				loadMTL(path, mtl_tex_path, atlas, iss);
			}
			else if (prefix == "usemtl")
			{
				std::string	mtllib;
				if (!(iss >> mtllib))
					throw std::runtime_error("invalid usemtl");

				std::string tex_path = mtl_tex_path[mtllib];

				if (!atlas.has(tex_path))
				{
					std::cout << mtllib << std::endl;
					continue ;
				}

				current_uv_off = atlas.uv(tex_path);
			}
		}
	}
	catch (const std::exception &e)
	{
		throw std::runtime_error(path + " " + std::string(e.what()) + " - line " + std::to_string(line_number));
	}
}
