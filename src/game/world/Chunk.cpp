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

void	Chunk::draw(const mbl::render::Camera& cam, bool draw_bounds)
{
	if (_blockMesh.vertices() == 0)
		return ;

	mbl::render::Shader*		shader = _ext_shader ? _ext_shader : &_int_shader;

	if (_need_upload)
	{
		_blockMesh.upload();
		_need_upload = false;
		_state = State::UPLOADED;
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

void	Chunk::generate(u32 seed)
{
	Noise::Perlin2D	noise;
	noise.init(seed);

	chunkLocalVec3i	blockPos;
	for (blockPos.x() = 0; blockPos.x() < Chunk::SIZE; blockPos.x()++)
		for (blockPos.z() = 0; blockPos.z() < Chunk::SIZE; blockPos.z()++)
		{
			worldVec3i worldPos = chunkLocalToWorld(blockPos, _pos, Chunk::SIZE);
			int	y = -noise.sample_turbulence_fbm(vec2f(worldPos.x(), worldPos.z()), 0.00125, 6) * 1280;

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
	_state = State::GENERATED;
}

void	Chunk::mesh(BlockRegistry& blocks, std::array<chunkPtr, 6> neighbours)
{
	_blockMesh.add_vertex_layout(0, 3, GL_FLOAT, offsetof(Vertex, pos));
	_blockMesh.add_vertex_layout(1, 3, GL_FLOAT, offsetof(Vertex, normal));
	_blockMesh.add_vertex_layout(2, 3, GL_FLOAT, offsetof(Vertex, color));
	_blockMesh.add_vertex_layout(3, 2, GL_FLOAT, offsetof(Vertex, uv));
	_blockMesh.set_sizeof_layout(sizeof(Vertex));

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
					BlockModel& model = blocks.getState(block).model();
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
					}
				}
			}
		}
	}
	_state = State::MESHED;
	_need_upload = true;
}
