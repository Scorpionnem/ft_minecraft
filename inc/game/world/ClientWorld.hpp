#pragma once

#include "game/world/World.hpp"
#include "game/world/render/ChunkRender.hpp"

class	ClientWorld : public World
{
	public:
		static constexpr double	CHUNK_REQUEST_TIMEOUT = 0.75;
	public:
		struct	OctreeNode
		{
			enum Branch
			{
				B_000,
				B_100,
				B_010,
				B_110,
				B_001,
				B_101,
				B_011,
				B_111,
				OUT_OF_BOUNDS,
			};
			OctreeNode::Branch	getBranchId(vec3i pos)
			{
				if (!mbl::utils::aabb3i::contains({.pos = _pos, .size = _size}, pos))
					return (Branch::OUT_OF_BOUNDS);

				bool	isMinX = pos.x() < _pos.x() + _size.x() / 2;
				bool	isMinY = pos.y() < _pos.y() + _size.y() / 2;
				bool	isMinZ = pos.z() < _pos.z() + _size.z() / 2;
				bool	isPosX = !isMinX;
				bool	isPosY = !isMinY;
				bool	isPosZ = !isMinZ;

				if (isMinX && isMinY && isMinZ)
					return (Branch::B_000);
				if (isPosX && isPosY && isPosZ)
					return (Branch::B_111);
				if (isPosX && isMinY && isMinZ)
					return (Branch::B_100);
				if (isMinX && isPosY && isMinZ)
					return (Branch::B_010);
				if (isMinX && isMinY && isPosZ)
					return (Branch::B_001);
				if (isPosX && isMinY && isPosZ)
					return (Branch::B_101);
				if (isMinX && isPosY && isPosZ)
					return (Branch::B_011);
				if (isPosX && isPosY && isMinZ)
					return (Branch::B_110);
				return (Branch::OUT_OF_BOUNDS);
			}
			vec3i	childPos(Branch id) const
			{
				vec3i half = _size / 2;
				if (id == B_000)
					return (_pos + vec3i(0, 0, 0));
				if (id == B_111)
					return (_pos + vec3i(half.x(), half.y(), half.z()));
				if (id == B_011)
					return (_pos + vec3i(0, half.y(), half.z()));
				if (id == B_101)
					return (_pos + vec3i(half.x(), 0, half.z()));
				if (id == B_110)
					return (_pos + vec3i(half.x(), half.y(), 0));
				if (id == B_001)
					return (_pos + vec3i(0, 0, half.z()));
				if (id == B_100)
					return (_pos + vec3i(half.x(), 0, 0));
				if (id == B_010)
					return (_pos + vec3i(0, half.y(), 0));
				return (0);
			}

			OctreeNode(OctreeNode* parent, vec3i pos)
			{
				_parent = parent;
				_pos = pos;
				if (_parent)
				{
					_size = _parent->_size / 2;
					_depth = _parent->_depth + 1;
				}
				else
				{
					_size = 0;
					_depth = 0;
				}
			}

			void	draw(const mbl::render::Camera& cam, i32 depth = -1)
			{
				if (depth == -1 || this->_depth == depth)
				{
					srand(_depth);
					vec3f	rand_color = vec3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
					mbl::render::renderer::AABBRenderer::draw(mbl::utils::aabb3i{.pos = _pos, .size = _size}, cam, rand_color);
				}
				if (_chunk_render)
					_chunk_render->draw(cam, false);
				for (auto c : _children)
					if (c)
						c->draw(cam, depth);
			}

			void	appendNode(std::shared_ptr<ChunkRender> cr)
			{
				Branch	id = getBranchId(cr->pos() * Chunk::SIZE);
				if (id == Branch::OUT_OF_BOUNDS)
					return ;

				if (!_children[id])
					_children[id] = std::make_shared<OctreeNode>(this, childPos(id));

				if (_children[id]->_size == Chunk::SIZE)
					_children[id]->_chunk_render = cr;
				else
					_children[id]->appendNode(cr);
			}

			vec3i	_pos = {};
			vec3i	_size = {};
			i32		_depth = -1;

			std::shared_ptr<ChunkRender>							_chunk_render;
			std::array<std::shared_ptr<OctreeNode>, 8>				_children;
			OctreeNode*												_parent;
		};
		using OctreeNodePtr = std::shared_ptr<OctreeNode>;

		OctreeNodePtr	root;

		ClientWorld()
		{
			root = std::make_shared<OctreeNode>(nullptr, vec3i(0, 0, 0));
			root->_size = pow(32, 4);
		}

		void	update();
		void	draw(const chunkWorldVec3i& center_chunk, u16 render_distance, const mbl::render::Camera& cam, bool debug, int octree_depth = -1);

		void	meshChunk(chunkPtr c);

		bool	requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net, int& tx_pckt);
		void	requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, int& tx_pckt);

		void	netChunkData(const Packet::ChunkData* pckt);
		void	netChunkDataSpecial(const Packet::ChunkDataSpecial* pckt);

		void	setAtlas(mbl::render::TextureAtlas* atlas) {_atlas = atlas; _blocks.setAtlas(atlas);}

		std::unordered_map<chunkPosHash, chunkPtr>::iterator	removeChunk(const chunkWorldVec3i& pos)
		{
			_chunkRequests.erase(hash(pos));
			_meshes.erase(hash(pos));
			return (World::removeChunk(pos));
		}
	private:
		struct	PendingChunk
		{
			chunkPtr	chunk;
			u64			receivedMask = 0;
			mbl::utils::Chrono	time = {};
		};

		void	_netChunkReceived(PendingChunk& pending);

		mbl::render::TextureAtlas*	_atlas = nullptr;
		std::map<chunkPosHash, std::shared_ptr<ChunkRender>>	_meshes;

		std::unordered_map<chunkPosHash, PendingChunk>	_chunkRequests;
};
