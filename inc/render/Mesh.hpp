#pragma once

#include "math.hpp"
#include "render/Shader.hpp"

#include <vector>

class   Mesh
{
    public:
        struct	VertexLayout
		{
			// Location (layout (location = X))
			u32	location;

			// Number of .type in the layout (3 for a vec3f)
			u32	count;
			// Type of the layout (GL_FLOAT for a vec3f)
			GLenum		type;

			// Offset of layout in struct
			u64	offset;
		};
    public:
        Mesh() {}
        ~Mesh()
        {
            if (_VAO != 0)
				glDeleteVertexArrays(1, &_VAO);
			if (_VBO != 0)
				glDeleteBuffers(1, &_VBO);
        }

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

		void	add_vertex_data(u8 *bytes, u64 size)
		{
            _vertices += size / _sizeof_layout;
			_mesh_bytes.insert(_mesh_bytes.end(), bytes, bytes + size);
		}
		void	add_vertex_layout(u32 location, u32 count, GLenum type, u32 offset)
		{
			_vertex_layouts.push_back(VertexLayout{.location = location, .count = count, .type = type, .offset = offset});
		}
		void	set_sizeof_layout(u64 size)
		{
			_sizeof_layout = size;
		}

        void	upload()
		{
			if (_VAO != 0)
				glDeleteVertexArrays(1, &_VAO);
			if (_VBO != 0)
				glDeleteBuffers(1, &_VBO);

			glGenVertexArrays(1, &_VAO);
			glGenBuffers(1, &_VBO);

			glBindVertexArray(_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, _mesh_bytes.size(), _mesh_bytes.data(), GL_STATIC_DRAW);

			for (auto &v : _vertex_layouts)
			{
				glVertexAttribPointer(v.location, v.count, v.type, GL_FALSE, _sizeof_layout, (void*)v.offset);
				glEnableVertexAttribArray(v.location);
			}

			glBindVertexArray(0);
		}
		void	draw()
		{
			glBindVertexArray(_VAO);
			glDrawArrays(GL_TRIANGLES, 0, _vertices);
			glBindVertexArray(0);
		}
    private:
    	u32				_VAO = 0;
		u32				_VBO = 0;

		std::vector<u8>		_mesh_bytes;
		uint64_t			_vertices = 0;

		u32					        _sizeof_layout = 0;
		std::vector<VertexLayout>	_vertex_layouts;
};
