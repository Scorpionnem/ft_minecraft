#include "loader/OBJLoader.hpp"

#include <fstream>
#include <sstream>

static void	parse_vert(std::istringstream& iss, std::vector<vec3f>& pos_vec)
{
	float x, y, z;
	if (!(iss >> x >> y >> z))
		throw std::runtime_error("Invalid vertex");
	pos_vec.push_back(vec3f(x, y, z));
}

static void	parse_face(Mesh& mesh, std::istringstream& iss, std::vector<vec3f>& pos_vec)
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
			throw std::runtime_error("Invalid vertice index");
		vec3f	pos1 = pos_vec[fv1.pos_index];
		vec3f	pos2 = pos_vec[fv2.pos_index];
		vec3f	pos3 = pos_vec[fv3.pos_index];

		// if (fv1.uv_index < 0 || fv2.uv_index < 0 || fv3.uv_index < 0
		// 	|| fv1.uv_index > (int)pos_vec.size() - 1
		// 	|| fv2.uv_index > (int)pos_vec.size() - 1
		// 	|| fv3.uv_index > (int)pos_vec.size() - 1)
		// 	throw std::runtime_error("Invalid vertice index");
		vec2f	uv1 = vec2f(0);
		vec2f	uv2 = vec2f(0);
		vec2f	uv3 = vec2f(0);

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

void	OBJLoader::load(const std::string& path, Mesh& mesh)
{
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Failed to open model");

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

	std::string	line;
	u32			line_number = 0;

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
		else if (prefix == "f")
		{
			parse_face(mesh, iss, pos_vec);
		}
	}

}
