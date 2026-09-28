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
		}
		void	clear()
		{
			_blockMesh.clear();
			_blocks.fill(Block::AIR);
			_non_air_blocks = 0;
			_need_upload = false;
			_need_remesh = false;
			_pos = {};
			_state = State::NONE;
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

		void	generate(u32 seed/*Generator*/);
		void	mesh(BlockRegistry& blocks, std::array<chunkPtr, 6> neighbours);

		void	update();
		void	draw(const mbl::render::Camera& cam, bool draw_bounds = false);

		void	setPos(const chunkWorldVec3i& pos) {_pos = pos;}
		chunkWorldVec3i	pos() const {return (_pos);}

		Chunk::State	state() {return (_state);}
		void			setState(Chunk::State state) {_state = state;}

		bool	busy() const {return (_busy);}
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
		std::atomic<int>				_busy = 0;

		std::atomic<Chunk::State>		_state = Chunk::State::NONE;
};
