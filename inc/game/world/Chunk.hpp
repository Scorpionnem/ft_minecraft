#pragma once

#include "math/math.hpp"
#include "game/world/utils/Positions.hpp"
#include "game/world/Block.hpp"
#include "mbl.hpp"
#include "game/world/render/mesh/ChunkMeshData.hpp"

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
		using	Vertex = ChunkVertex;
	public:
		static constexpr int SIZE = 32;
		static constexpr int VOLUME = (Chunk::SIZE * Chunk::SIZE * Chunk::SIZE);

		static constexpr u32 BLOCKS_PER_PACKET = 512;
		static constexpr u32 PACKET_COUNT = Chunk::VOLUME / Chunk::BLOCKS_PER_PACKET;
	public:
		Chunk()
		{
			clear();
		}
		void	clear()
		{
			_meshData.clear();
			_blocks.fill(Block::AIR);
			_non_air_blocks = 0;
			_need_upload = false;
			_pos = {};
			_state = State::NONE;
		}
		~Chunk() {}

		void	generate(u32 seed/*Generator*/);
		void	mesh(BlockRegistry& blocks, std::array<chunkPtr, 6> neighbours);

		u32	process_non_air()
		{
			_non_air_blocks = 0;
			for (auto& b : _blocks)
				if (b != Block::AIR)
					_non_air_blocks++;
			return (_non_air_blocks);
		}

		void	setMeshData(const ChunkMeshData& data)
		{
			_meshData = data;
			_state = State::MESHED;
			_need_upload = true;
		}
		ChunkMeshData&	meshData() {return (_meshData);}

		void	update();

		void			setPos(const chunkWorldVec3i& pos) {_pos = pos;}
		chunkWorldVec3i	pos() const {return (_pos);}

		Chunk::State	state() {return (_state);}
		void			setState(Chunk::State state) {_state = state;}

		bool	busy() const {return (_busy);}
		void	setBusy(bool state) {_busy = state;}

		std::array<blockStateId, Chunk::VOLUME>&	data() {return (_blocks);}
		const std::array<blockStateId, Chunk::VOLUME>&	data() const {return (_blocks);}

		bool	empty() const {return (_non_air_blocks == 0);}
		u32		non_air_blocks() const {return (_non_air_blocks);}

		static constexpr uint16_t	blockIndex(const chunkLocalVec3i &pos) {return (pos.x() + pos.y() * Chunk::SIZE + pos.z() * Chunk::SIZE * Chunk::SIZE);}
		static constexpr bool		isInBounds(const chunkLocalVec3i &pos) {return (pos.x() >= 0 && pos.y() >= 0 && pos.z() >= 0 && pos.x() < Chunk::SIZE && pos.y() < Chunk::SIZE && pos.z() < Chunk::SIZE);}

		inline blockStateId	getBlock(const chunkLocalVec3i &pos) const
		{
			if (!_isInBounds(pos))
				return (Block::AIR);
			return (_blocks[blockIndex(pos)]);
		}
		inline void			setBlock(const chunkLocalVec3i &pos, blockStateId block)
		{
			if (!_isInBounds(pos))
				return ;
			uint16_t idx = blockIndex(pos);
			blockStateId old = _blocks[idx];
			_blocks[idx] = block;
			if (old == 0 && block != 0)
				_non_air_blocks++;
			else if (old != 0 && block == 0)
				_non_air_blocks--;
		}

	private:
		inline blockStateId	_getBlockUnsafe(const chunkLocalVec3i &pos) const {return (getBlock(pos));}
		inline void			_setBlockUnsafe(const chunkLocalVec3i &pos, blockStateId block) {setBlock(pos, block);}
		inline uint16_t		_blockIndex(const chunkLocalVec3i &pos) const {return (blockIndex(pos));}
		inline bool			_isInBounds(const chunkLocalVec3i &pos) const {return (isInBounds(pos));}
	private:
		chunkWorldVec3i							_pos = {};
		std::array<blockStateId, Chunk::VOLUME>	_blocks = {};

		u32	_non_air_blocks = 0;

		ChunkMeshData					_meshData;
		bool							_need_upload = false;

		// is chunk in a thread
		std::atomic<int>				_busy = 0;

		std::atomic<Chunk::State>		_state = Chunk::State::NONE;
};
