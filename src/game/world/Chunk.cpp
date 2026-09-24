#include "game/world/Chunk.hpp"

mbl::render::Shader*	Chunk::_ext_shader;
mbl::render::Shader		Chunk::_int_shader;
mbl::render::TextureAtlas*	Chunk::_texture;

const vec2f	UV00(0.f, 0.f);
const vec2f	UV10(1.f, 0.f);
const vec2f	UV11(1.f, 1.f);
const vec2f	UV01(0.f, 1.f);
const vec3f	V1(0, 1, 1);
const vec3f	V2(1, 1, 1);
const vec3f	V3(1, 1, 0);
const vec3f	V4(0, 1, 0);
const vec3f	V5(0, 0, 1);
const vec3f	V6(1, 0, 1);
const vec3f	V7(1, 0, 0);
const vec3f	V8(0, 0, 0);

const vec3i	DIR_OFFSET[6] =
{
	vec3i(0, 1, 0), // TOP
	vec3i(0, -1, 0), // BOTTOM
	vec3i(0, 0, 1), // NORTH
	vec3i(0, 0, -1), // SOUTH
	vec3i(1, 0, 0), // EAST
	vec3i(-1, 0, 0), // WEST
};

Chunk::Face	FACE1[6] =
{
	Chunk::Face( // TOP
		Chunk::Vertex(V1, vec3f(0,1,0), UV01),
		Chunk::Vertex(V2, vec3f(0,1,0), UV11),
		Chunk::Vertex(V4, vec3f(0,1,0), UV00)
	),
	Chunk::Face( // BOTTOM
		Chunk::Vertex(V8, vec3f(0,-1,0), UV01),
		Chunk::Vertex(V6, vec3f(0,-1,0), UV10),
		Chunk::Vertex(V5, vec3f(0,-1,0), UV00)
	),
	Chunk::Face( // NORTH
		Chunk::Vertex(V5, vec3f(0,0,1), UV00),
		Chunk::Vertex(V2, vec3f(0,0,1), UV11),
		Chunk::Vertex(V1, vec3f(0,0,1), UV01)
	),
	Chunk::Face( // SOUTH
		Chunk::Vertex(V8, vec3f(0,0,-1), UV00),
		Chunk::Vertex(V4, vec3f(0,0,-1), UV01),
		Chunk::Vertex(V3, vec3f(0,0,-1), UV11)
	),
	Chunk::Face( // EAST
		Chunk::Vertex(V3, vec3f(1,0,0), UV11),
		Chunk::Vertex(V2, vec3f(1,0,0), UV01),
		Chunk::Vertex(V6, vec3f(1,0,0), UV00)
	),
	Chunk::Face( // WEST
		Chunk::Vertex(V5, vec3f(-1,0,0), UV00),
		Chunk::Vertex(V1, vec3f(-1,0,0), UV01),
		Chunk::Vertex(V4, vec3f(-1,0,0), UV11)
	),
};

Chunk::Face	FACE2[6] =
{
	Chunk::Face( // TOP
		Chunk::Vertex(V2, vec3f(0,1,0), UV11),
		Chunk::Vertex(V3, vec3f(0,1,0), UV10),
		Chunk::Vertex(V4, vec3f(0,1,0), UV00)
	),
	Chunk::Face( // BOTTOM
		Chunk::Vertex(V8, vec3f(0,-1,0), UV01),
		Chunk::Vertex(V7, vec3f(0,-1,0), UV11),
		Chunk::Vertex(V6, vec3f(0,-1,0), UV10)
	),
	Chunk::Face( // NORTH
		Chunk::Vertex(V5, vec3f(0,0,1), UV00),
		Chunk::Vertex(V6, vec3f(0,0,1), UV10),
		Chunk::Vertex(V2, vec3f(0,0,1), UV11)
	),
	Chunk::Face( // SOUTH
		Chunk::Vertex(V8, vec3f(0,0,-1), UV00),
		Chunk::Vertex(V3, vec3f(0,0,-1), UV11),
		Chunk::Vertex(V7, vec3f(0,0,-1), UV10)
	),
	Chunk::Face( // EAST
		Chunk::Vertex(V3, vec3f(1,0,0), UV11),
		Chunk::Vertex(V6, vec3f(1,0,0), UV00),
		Chunk::Vertex(V7, vec3f(1,0,0), UV10)
	),
	Chunk::Face( // WEST
		Chunk::Vertex(V5, vec3f(-1,0,0), UV00),
		Chunk::Vertex(V4, vec3f(-1,0,0), UV11),
		Chunk::Vertex(V8, vec3f(-1,0,0), UV10)
	),
};

void	Chunk::mesh()
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

				BlockState	block = _getBlockUnsafe(blockPos);

				if (block != 0)
				{
					for (int dir = 0; dir < 6; dir++)
					{
						BlockState	cull_block = 0;

						chunkLocalVec3i	thisChunkPos = blockPos + DIR_OFFSET[dir];

						if (!_isInBounds(thisChunkPos))
							continue ;

						if (_isInBounds(thisChunkPos))
							cull_block = _getBlockUnsafe(thisChunkPos);

						if (cull_block != 0)
							continue ;

						Face	f1 = FACE1[dir];
						Face	f2 = FACE2[dir];
						f1.v1.pos = (vec3f)blockPos + f1.v1.pos;
						f1.v2.pos = (vec3f)blockPos + f1.v2.pos;
						f1.v3.pos = (vec3f)blockPos + f1.v3.pos;
						f2.v1.pos = (vec3f)blockPos + f2.v1.pos;
						f2.v2.pos = (vec3f)blockPos + f2.v2.pos;
						f2.v3.pos = (vec3f)blockPos + f2.v3.pos;

						_blockMesh.add_vertex_data(reinterpret_cast<uint8_t*>(&f1), sizeof(f1));
						_blockMesh.add_vertex_data(reinterpret_cast<uint8_t*>(&f2), sizeof(f2));
					}
				}
			}
		}
	}
	_need_upload = true;
}
