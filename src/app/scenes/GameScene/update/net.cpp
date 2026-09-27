#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"

void	GameScene::_dispatch_packet(u8 *data, u64 size)
{
	if (size < sizeof(Packet::Header))
		return ;

	Packet::Header*	hdr = reinterpret_cast<Packet::Header*>(data);
	if (hdr->magic != MINECRAFT_PCKT_MAGIC) // invalid packet
		return ;

	_rx_pckt++;

	switch (hdr->type)
	{
		case CHUNKDATA_TYPE:
		{
			Packet::ChunkData*	chunk_pckt = reinterpret_cast<Packet::ChunkData*>(data);

			_world.netChunkData(chunk_pckt);
			break ;
		}
		case CHUNKDATASPECIAL_TYPE:
		{
			Packet::ChunkDataSpecial*	chunk_pckt = reinterpret_cast<Packet::ChunkDataSpecial*>(data);

			_world.netChunkDataSpecial(chunk_pckt);
			break ;
		}
		default :
			return ;
	}
}

void	GameScene::_update_net()
{
	mbl::net::Client::Event	event;
	u8					buf[4096] = {};
	u64					size;

	int	packets_recvd = 0;

	_netClient.update();

	do
	{
		if (_netClient.recv(buf, sizeof(buf), event, size) == -1)
			throw std::runtime_error(strerror(errno));
		if (event == mbl::net::Client::Event::DISCONNECT)
		{
			_netClient.disconnect();
			throw std::runtime_error("Disconnected");
		}
		if (event == mbl::net::Client::Event::RECV)
		{
			_dispatch_packet(buf, size);

			packets_recvd++;
		}
	} while (event != mbl::net::Client::Event::NONE && packets_recvd < MAX_PACKETS_PER_FRAME);
}
