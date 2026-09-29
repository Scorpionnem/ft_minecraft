#include "game/world/render/mesh/ChunkMesher.hpp"
#include "game/world/Chunk.hpp"
#include "game/world/Block.hpp"

static const vec3i DIR_OFFSET[6] =
{
	vec3i(0, 1, 0),  // TOP
	vec3i(0, -1, 0), // BOTTOM
	vec3i(0, 0, 1),  // NORTH
	vec3i(0, 0, -1), // SOUTH
	vec3i(1, 0, 0),  // EAST
	vec3i(-1, 0, 0), // WEST
};

ChunkMeshData ChunkMesher::buildMesh(const Chunk& chunk,
									 const std::array<chunkPtr, 6>& neighbours,
									 BlockRegistry& blocks)
{
	ChunkMeshData meshData;

	chunkLocalVec3i blockPos;
	for (blockPos.x() = 0; blockPos.x() < Chunk::SIZE; blockPos.x()++)
	{
		for (blockPos.y() = 0; blockPos.y() < Chunk::SIZE; blockPos.y()++)
		{
			for (blockPos.z() = 0; blockPos.z() < Chunk::SIZE; blockPos.z()++)
			{
				if (!Chunk::isInBounds(blockPos))
					continue;

				blockStateId block = chunk.getBlock(blockPos);

				if (block != Block::AIR)
				{
					BlockModel& model = blocks.getState(block).model();
					std::array<i64, 6> cull_neighbours;

					for (int dir = 0; dir < 6; dir++)
					{
						cull_neighbours[dir] = -1;

						chunkLocalVec3i thisChunkPos = blockPos + DIR_OFFSET[dir];
						chunkLocalVec3i neighbourChunkPos = blockPos + DIR_OFFSET[dir] - (vec3i(Chunk::SIZE) * DIR_OFFSET[dir]);

						if (!Chunk::isInBounds(thisChunkPos) && !neighbours[dir])
							continue;

						if (Chunk::isInBounds(thisChunkPos))
							cull_neighbours[dir] = chunk.getBlock(thisChunkPos);
						else if (neighbours[dir] && Chunk::isInBounds(neighbourChunkPos))
							cull_neighbours[dir] = neighbours[dir]->getBlock(neighbourChunkPos);
					}

					for (auto& [name, f] : model._faces)
					{
						if (f.cull_face == mbl::utils::FacingCardinal::INVALID
							|| (f.cull_face != mbl::utils::FacingCardinal::INVALID && cull_neighbours[(int)f.cull_face] != -1 && !blocks.getState(cull_neighbours[(int)f.cull_face]).getBlock().solid()))
						{
							for (const auto& v : f.vertices)
							{
								meshData.vertices.push_back(ChunkVertex{
									.pos = v.pos + blockPos,
									.normal = v.normal,
									.color = v.color,
									.uv = v.uv,
								});
							}
						}
					}
				}
			}
		}
	}

	return meshData;
}
