#pragma once

#include "math/math.hpp"
#include "game/world/utils/Positions.hpp"
#include "mbl.hpp"

#include <array>

using BlockState = u32;

class	Chunk
{
	public:
		struct	Vertex
		{
			Vertex() {}
			Vertex(vec3f pos, vec3f normal, vec2f uv)
			{
				this->pos = pos;
				this->normal = normal;
				this->uv = uv;
			}
			vec3f	pos;
			vec3f	normal;
			vec2f	uv;
		};

		struct	Face
		{
			Face() {}
			Face(Chunk::Vertex v1, Chunk::Vertex v2, Chunk::Vertex v3)
			{
				this->v1 = v1;
				this->v2 = v2;
				this->v3 = v3;
			}
			Chunk::Vertex	v1;
			Chunk::Vertex	v2;
			Chunk::Vertex	v3;
		};
	public:
		static constexpr u32 SIZE = 32;
		static constexpr u32 VOLUME = (Chunk::SIZE * Chunk::SIZE * Chunk::SIZE);
	public:
		Chunk()
		{
			_blockMesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(Vertex, pos));
			_blockMesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(Vertex, normal));
			_blockMesh.add_vertex_layout(2, 2, GL_FLOAT, offsetof(Vertex, uv));
			_blockMesh.set_sizeof_layout(sizeof(Vertex));
		}
		~Chunk() {}

		static void	load_shader(const char* vert_path = "assets/shaders/chunk.vert", const char* frag_path = "assets/shaders/chunk.frag",
			mbl::render::Shader* ext_shader = nullptr)
		{
			_ext_shader = ext_shader;
			mbl::render::Shader*	shader = _ext_shader ? _ext_shader : &_int_shader;

			if (!_ext_shader)
				shader->load(vert_path, frag_path);
		}

		void	generate(/*Generator*/)
		{
			chunkLocalVec3i	blockPos;
			for (blockPos.x() = 0; blockPos.x() < Chunk::SIZE; blockPos.x()++)
				for (blockPos.z() = 0; blockPos.z() < Chunk::SIZE; blockPos.z()++)
				{
					worldVec3i worldPos = chunkLocalToWorld(_pos, blockPos, Chunk::SIZE);
					float	scale = 256.0f;
					int	y = (sinf((worldPos.x()) / scale) + cosf(worldPos.z() / scale)) * 32;

					for (blockPos.y() = 0; blockPos.y() < Chunk::SIZE; blockPos.y()++)
					{
						if (blockPos.y() < y)
							_setBlockUnsafe(blockPos, 1);
					}
				}
		}
		void	mesh();

		void	update()
		{

		}
		void	draw(const mbl::render::Camera& cam, bool draw_bounds = false)
		{
			mbl::render::Shader*		shader = _ext_shader ? _ext_shader : &_int_shader;

			if (_need_upload)
			{
				_blockMesh.upload();
				_need_upload = false;
			}

			shader->bind();
			shader->setMat4("uView", cam.getViewMatrix());
			shader->setMat4("uProj", cam.getProjectionMatrix());
			shader->setMat4("uModel", mat4f::translate(_pos));
			shader->setInt("uAtlas", 0);
			_blockMesh.draw(GL_TRIANGLES);

			if (draw_bounds)
				mbl::render::renderer::AABBRenderer::draw(mbl::utils::aabb3f{.pos = _pos, .size = vec3f(Chunk::SIZE)}, cam, vec3f(0, 1, 0));
		}

		void	setPos(const chunkWorldVec3i& pos) {_pos = pos;}
		chunkWorldVec3i	pos() const {return (_pos);}
	private:
		inline BlockState	_getBlockUnsafe(const chunkLocalVec3i &pos) {return (_blocks[_blockIndex(pos)]);}
		inline void			_setBlockUnsafe(const chunkLocalVec3i &pos, BlockState block) {_blocks[_blockIndex(pos)] = block;}
		inline uint16_t		_blockIndex(const chunkLocalVec3i &pos) {return (pos.x() + pos.y() * Chunk::SIZE + pos.z() * Chunk::SIZE * Chunk::SIZE);}
		inline bool			_isInBounds(const chunkLocalVec3i &pos) {return (pos.x() >= 0 && pos.y() >= 0 && pos.z() >= 0 && pos.x() < Chunk::SIZE && pos.y() < Chunk::SIZE && pos.z() < Chunk::SIZE);}
	private:
		static mbl::render::Shader*			_ext_shader;
		static mbl::render::Shader			_int_shader;
		static mbl::render::TextureAtlas*	_texture;

		chunkWorldVec3i							_pos;
		std::array<BlockState, Chunk::VOLUME>	_blocks;

		bool							_need_upload = false;
		mbl::render::Mesh				_blockMesh;
};
