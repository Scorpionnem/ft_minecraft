#pragma once

#include "game/world/World.hpp"
#include "game/world/render/ChunkRender.hpp"

class	ClientWorld : public World
{
	public:
		static constexpr double	CHUNK_REQUEST_TIMEOUT = 0.75;
	public:
		void	update();
		void	draw(const chunkWorldVec3i& center_chunk, u16 render_distance, const mbl::render::Camera& cam, bool debug);

		void	meshChunk(chunkPtr c);

		bool	requestChunk(const chunkWorldVec3i& pos, mbl::net::Client& net, int& tx_pckt);
		void	requestInRange(const chunkWorldVec3i& center_chunk, u16 render_distance, mbl::net::Client& net, int& tx_pckt);

		void	netChunkData(const Packet::ChunkData* pckt);
		void	netChunkDataSpecial(const Packet::ChunkDataSpecial* pckt);

		void	setAtlas(mbl::render::TextureAtlas* atlas) {_atlas = atlas; _blocks.setAtlas(atlas);}

		void	removeChunk(const chunkWorldVec3i& pos)
		{
			_chunkRequests.erase(hash(pos));
			World::removeChunk(pos);
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
