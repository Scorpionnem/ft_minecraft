#pragma once

#include "math/math.hpp"
#include "game/world/utils/Positions.hpp"
#include "game/world/Block.hpp"
#include "mbl.hpp"
#include "game/world/generation/noise/Noise.hpp"

#include <array>

using chunkPosHash = u64;
using chunkPtr = std::shared_ptr<class Chunk>;

class	Chunk
{
	public:
		enum	State
		{
			NONE,
			GENERATED,
			MESHED,
			UPLOADED,
		};
		struct	Vertex
		{
			vec3f	pos;
			vec3f	normal;
			vec3f	color;
			vec2f	uv;
		};
	public:
		static constexpr int SIZE = 32;
		static constexpr int VOLUME = (Chunk::SIZE * Chunk::SIZE * Chunk::SIZE);

		static constexpr u32 BLOCKS_PER_PACKET = 512;
		static constexpr u32 PACKET_COUNT = Chunk::VOLUME / Chunk::BLOCKS_PER_PACKET;
	public:
		Chunk()
		{
			_busy = false;
			_blockMesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(Vertex, pos));
			_blockMesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(Vertex, normal));
			_blockMesh.add_vertex_layout(2, 3, GL_FLOAT, offsetof(Vertex, color));
			_blockMesh.add_vertex_layout(3, 2, GL_FLOAT, offsetof(Vertex, uv));
			_blockMesh.set_sizeof_layout(sizeof(Vertex));
		}
		void	clear()
		{
			_busy = false;
			_non_air_blocks = 0;
			_blockMesh.clear();
			_blocks = {};
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

		void	generate(u32 seed/*Generator*/)
		{
			Noise::Perlin2D	noise;
			noise.init(seed);

			chunkLocalVec3i	blockPos;
			for (blockPos.x() = 0; blockPos.x() < Chunk::SIZE; blockPos.x()++)
				for (blockPos.z() = 0; blockPos.z() < Chunk::SIZE; blockPos.z()++)
				{
					worldVec3i worldPos = chunkLocalToWorld(blockPos, _pos, Chunk::SIZE);
					int	y = -noise.sample_turbulence_fbm(vec2f(worldPos.x(), worldPos.z()), 0.0025, 4) * 320;

					for (blockPos.y() = 0; blockPos.y() < Chunk::SIZE; blockPos.y()++)
					{
						worldVec3i worldPos2 = chunkLocalToWorld(blockPos, _pos, Chunk::SIZE);

						float	noise = Noise::rand2dTo1d(vec2i(worldPos.x(), worldPos.z()));
						if (worldPos2.y() == y + 1 && noise < 0.01)
							_setBlockUnsafe(blockPos, Block::BLUE_ORCHID);
						else if (worldPos2.y() == y + 1 && noise < 0.1)
							_setBlockUnsafe(blockPos, Block::GRASS);
						if (worldPos2.y() == y)
							_setBlockUnsafe(blockPos, Block::GRASS_BLOCK);
						else if (worldPos2.y() < y)
							_setBlockUnsafe(blockPos, Block::STONE);
					}
				}
		}
		void	mesh(BlockRegistry& blocks, std::array<chunkPtr, 6> neighbours);

		void	update()
		{

		}
		void	draw(const mbl::render::Camera& cam, bool draw_bounds = false)
		{
			if (_blockMesh.vertices() == 0)
				return ;

			mbl::render::Shader*		shader = _ext_shader ? _ext_shader : &_int_shader;

			if (_need_upload)
			{
				_blockMesh.upload();
				_need_upload = false;
			}

			shader->bind();
			shader->setMat4("uView", cam.getViewMatrix());
			shader->setMat4("uProj", cam.getProjectionMatrix());
			shader->setMat4("uModel", mat4f::translate(_pos * Chunk::SIZE));
			shader->setInt("uAtlas", 0);
			_blockMesh.draw(GL_TRIANGLES);

			if (draw_bounds)
				mbl::render::renderer::AABBRenderer::draw(mbl::utils::aabb3f{.pos = _pos * Chunk::SIZE, .size = vec3f(Chunk::SIZE)}, cam, vec3f(0, 1, 0));
		}

		void	setPos(const chunkWorldVec3i& pos) {_pos = pos;}
		chunkWorldVec3i	pos() const {return (_pos);}

		Chunk::State	state() {return (_state);}

		bool	busy() {return (_busy);}
		void	setBusy(bool state) {_busy = state;}

		bool	need_remesh() {return (_need_remesh);}
		void	setNeedRemesh(bool state) {_need_remesh = state;}

		std::array<blockStateId, Chunk::VOLUME>&	data() {return (_blocks);}
		bool	empty() {return (_non_air_blocks == 0);}
	private:
		inline blockStateId	_getBlockUnsafe(const chunkLocalVec3i &pos) {return (_blocks[_blockIndex(pos)]);}
		inline void			_setBlockUnsafe(const chunkLocalVec3i &pos, blockStateId block) {_blocks[_blockIndex(pos)] = block; if (block != 0) _non_air_blocks++;}
		inline uint16_t		_blockIndex(const chunkLocalVec3i &pos) {return (pos.x() + pos.y() * Chunk::SIZE + pos.z() * Chunk::SIZE * Chunk::SIZE);}
		inline bool			_isInBounds(const chunkLocalVec3i &pos) {return (pos.x() >= 0 && pos.y() >= 0 && pos.z() >= 0 && pos.x() < Chunk::SIZE && pos.y() < Chunk::SIZE && pos.z() < Chunk::SIZE);}
	private:
		static mbl::render::Shader*			_ext_shader;
		static mbl::render::Shader			_int_shader;
		static mbl::render::TextureAtlas*	_texture;

		chunkWorldVec3i							_pos = {};
		std::array<blockStateId, Chunk::VOLUME>	_blocks = {};

		u32	_non_air_blocks = 0;

		bool							_need_upload = false;
		mbl::render::Mesh				_blockMesh;

		std::atomic_bool				_need_remesh = false;

		// is chunk in a thread
		std::atomic_bool				_busy = false;

		std::atomic<Chunk::State>		_state = Chunk::State::NONE;
};
