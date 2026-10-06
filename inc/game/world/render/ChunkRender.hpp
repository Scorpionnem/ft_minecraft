#pragma once

#include "mbl.hpp"

#include "game/world/render/mesh/ChunkMeshData.hpp"
#include "game/world/Chunk.hpp"

class	ChunkRender
{
	public:
		ChunkRender(chunkWorldVec3i pos) : _pos(pos) {}
		~ChunkRender() {}

		void	upload(const ChunkMeshData& data)
		{
			_time.start();

			_mesh.clear();
			_mesh.set_sizeof_layout(sizeof(ChunkVertex));
			_mesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(ChunkVertex, pos));
			_mesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(ChunkVertex, normal));
			_mesh.add_vertex_layout(2, 3, GL_FLOAT, offsetof(ChunkVertex, color));
			_mesh.add_vertex_layout(3, 2, GL_FLOAT, offsetof(ChunkVertex, uv));
			_mesh.add_vertex_data(reinterpret_cast<const u8*>(data.vertices.data()), data.vertices.size() * sizeof(ChunkVertex));
			_mesh.upload();
		}

		static void	load_shader(const char* vert_path = "assets/shaders/chunk.vert", const char* frag_path = "assets/shaders/chunk.frag",
			mbl::render::Shader* ext_shader = nullptr)
		{
			_ext_shader = ext_shader;
			mbl::render::Shader*	shader = _ext_shader ? _ext_shader : &_int_shader;

			if (!_ext_shader)
				shader->load(vert_path, frag_path);
		}

		void	draw(const mbl::render::Camera& cam, bool debug)
		{
			mbl::render::Shader*		shader = _ext_shader ? _ext_shader : &_int_shader;

			if (_mesh.vertices() == 0)
				return ;

			if (_fade < 1)
				_fade = _time.get();
			else
				_fade = 1;

			shader->bind();
			shader->setMat4("uView", cam.getViewMatrix());
			shader->setMat4("uProj", cam.getProjectionMatrix());
			shader->setMat4("uModel", mat4f::translate(_pos * Chunk::SIZE));
			shader->setInt("uAtlas", 0);
			shader->setFloat("uFade", _fade);
			_mesh.draw(GL_TRIANGLES);

			if (debug)
				mbl::render::renderer::AABBRenderer::draw(mbl::utils::aabb3f{.pos = _pos * Chunk::SIZE, .size = vec3f(Chunk::SIZE)}, cam, vec3f(0, 1, 0));
		}

		chunkWorldVec3i	pos() {return (_pos);};
	private:
		static mbl::render::Shader*			_ext_shader;
		static mbl::render::Shader			_int_shader;

		chunkWorldVec3i		_pos;
		mbl::render::Mesh	_mesh;

		float	_fade = 0;
		mbl::utils::Chrono	_time;
};
