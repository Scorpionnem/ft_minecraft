#pragma once

#include "math.hpp"
#include "render/Mesh.hpp"
#include "render/Camera.hpp"

template <std::size_t N, typename T>
struct  AABB
{
    vec<N, T>   min;
    vec<N, T>   max;

    static bool    intersects(const AABB<N, T>& a, const AABB<N, T>& b)
    {
        for (std::size_t i = 0; i < N; i++)
            if (a.min.data[i] > b.max.data[i] || a.max.data[i] < b.min.data[i])
                return (false);
        return (true);
    }

    static bool    contains(const AABB<N, T>& box, const vec<N, T>& point)
    {
        for (std::size_t i = 0; i < N; i++)
            if (point.data[i] < box.min.data[i] || point.data[i] > box.max.data[i])
                return (false);
        return (true);
    }

    static void	gen_aabb3_mesh(Mesh& mesh) requires (N == 3)
	{
		mesh.set_sizeof_layout(2 * sizeof(vec3f));
		mesh.add_vertex_layout(0, 3, GL_FLOAT, 0);
		mesh.add_vertex_layout(1, 3, GL_FLOAT, sizeof(vec3f));

		vec3f vertices[] = {
			{0,0,0}, {0,0,0}, {1,0,0}, {0,0,0},
			{0,0,0}, {0,0,0}, {0,1,0}, {0,0,0},
			{0,0,0}, {0,0,0}, {0,0,1}, {0,0,0},
			{0,1,0}, {0,0,0}, {1,1,0}, {0,0,0},
			{0,1,0}, {0,0,0}, {0,1,1}, {0,0,0},
			{1,1,1}, {0,0,0}, {0,1,1}, {0,0,0},
			{1,1,1}, {0,0,0}, {1,0,1}, {0,0,0},
			{1,1,1}, {0,0,0}, {1,1,0}, {0,0,0},
			{0,1,1}, {0,0,0}, {0,0,1}, {0,0,0},
			{0,0,1}, {0,0,0}, {1,0,1}, {0,0,0},
			{1,0,1}, {0,0,0}, {1,0,0}, {0,0,0},
			{1,1,0}, {0,0,0}, {1,0,0}, {0,0,0},
		};

		mesh.add_vertex_data(reinterpret_cast<u8*>(vertices), sizeof(vertices));
	}

	template <typename TT>
	static void draw_aabb3(AABB<3, TT> aabb, Camera& cam, Shader& shader, Mesh& mesh)
	{
		shader.bind();
		shader.setMat4("uProj", cam.getProjectionMatrix());
		shader.setMat4("uView", cam.getViewMatrix());
		shader.setMat4("uModel", mat4f::translate(aabb.min) * mat4f::scale(aabb.max));
		mesh.draw(GL_LINES);
	}
};

using aabb2i = AABB<2, int>;
using aabb3i = AABB<3, int>;

using aabb2f = AABB<2, float>;
using aabb3f = AABB<3, float>;
