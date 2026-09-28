#include "game/world/Chunk.hpp"

mbl::render::Shader*	Chunk::_ext_shader = nullptr;
mbl::render::Shader		Chunk::_int_shader;
mbl::render::TextureAtlas*	Chunk::_texture = nullptr;

const vec3i	DIR_OFFSET[6] =
{
	vec3i(0, 1, 0), // TOP
	vec3i(0, -1, 0), // BOTTOM
	vec3i(0, 0, 1), // NORTH
	vec3i(0, 0, -1), // SOUTH
	vec3i(1, 0, 0), // EAST
	vec3i(-1, 0, 0), // WEST
};

void	Chunk::mesh(BlockRegistry& blocks, std::array<chunkPtr, 6> neighbours)
{
	chunkLocalVec3i	blockPos;
	for (blockPos.x() = 0; blockPos.x() < Chunk::SIZE; blockPos.x()++)
	{
		for (blockPos.y() = 0; blockPos.y() < Chunk::SIZE; blockPos.y()++)
		{
			for (blockPos.z() = 0; blockPos.z() < Chunk::SIZE; blockPos.z()++)
			{
				if (!_isInBounds(blockPos))
					continue ;

				blockStateId	block = _getBlockUnsafe(blockPos);

				if (block != Block::AIR)
				{
					BlockModel& model = blocks.getState(block).getBlock()._model;
					std::array<i64, 6> cull_neighbours;

					for (int dir = 0; dir < 6; dir++)
					{
						cull_neighbours[dir] = -1;

						chunkLocalVec3i	thisChunkPos = blockPos + DIR_OFFSET[dir];
						chunkLocalVec3i	neighbourChunkPos = blockPos + DIR_OFFSET[dir] - (vec3i(Chunk::SIZE) * DIR_OFFSET[dir]);

						if (!_isInBounds(thisChunkPos) && !neighbours[dir])
							continue ;

						if (_isInBounds(thisChunkPos))
							cull_neighbours[dir] = _getBlockUnsafe(thisChunkPos);
						else if (neighbours[dir]->_isInBounds(neighbourChunkPos))
							cull_neighbours[dir] = neighbours[dir]->_getBlockUnsafe(neighbourChunkPos);
					}

					for (auto& [name, f] : model._faces)
					{
						if (f.cull_face == mbl::utils::FacingCardinal::INVALID
							|| (f.cull_face != mbl::utils::FacingCardinal::INVALID && cull_neighbours[(int)f.cull_face] != -1 && !blocks.getState(cull_neighbours[(int)f.cull_face]).getBlock().solid()))
						{
							for (auto v : f.vertices)
							{
								v.pos += blockPos;
								_blockMesh.add_vertex_data(reinterpret_cast<uint8_t*>(&v), sizeof(v));
							}
						}
						// if ((f.cull_face != mbl::utils::FacingCardinal::INVALID && cull_neighbours[static_cast<int>(f.cull_face)] != Block::AIR)
						// 	|| (f.cull_face != mbl::utils::FacingCardinal::INVALID && cull_neighbours[static_cast<int>(f.cull_face)] == -1))
						// 	continue ;
					}
				}
			}
		}
	}
	_state = State::MESHED;
	_need_upload = true;
}
