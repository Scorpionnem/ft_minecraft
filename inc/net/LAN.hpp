#pragma once

#include <string>
#include "mbl.hpp"

#define DEFAULT_BROADCAST_ADDR "224.0.0.42"
#define DEFAULT_BROADCAST_PORT (6767)

struct	LANServer
{
	std::string	addr;
	int			port = 0;
	i64			last_updt_ts = 0;
};

#define MAX_NAME_LEN 16

namespace Packet
{
	#define MINECRAFT_PCKT_MAGIC (0xB00BB00B)
	struct	Header
	{
		u32	type = -1;
		u32	magic = MINECRAFT_PCKT_MAGIC;
	} __attribute__((packed));

	#define LANBROADCAST_TYPE 1
	#define LANBROADCAST_MAX_ADDR_LEN 32
	struct	LANBroadcast
	{
		Packet::Header	hdr = {.type = LANBROADCAST_TYPE};

		char			addr[32] = {};
		int				port = 0;
	} __attribute__((packed));

	#define CONNECT_TYPE 2
	struct	Connect // packet sent by player when connecting
	{
		Packet::Header	hdr = {.type = CONNECT_TYPE};

		char			name[MAX_NAME_LEN + 1] = {};
	} __attribute__((packed));

	#define ANNOUNCECONNECT_TYPE 3
	struct	AnnounceConnect // packet sent by server to announce new player
	{
		Packet::Header	hdr = {.type = ANNOUNCECONNECT_TYPE};

		char			name[MAX_NAME_LEN + 1] = {};
		u8				slot = 0; // player slot 0 -> 3
	} __attribute__((packed));

	#define ANNOUNCEDISCONNECT_TYPE 4
	struct	AnnounceDisconnect // packet sent by server to announce player removed
	{
		Packet::Header	hdr = {.type = ANNOUNCEDISCONNECT_TYPE};

		char			name[MAX_NAME_LEN + 1] = {};
	} __attribute__((packed));
};
