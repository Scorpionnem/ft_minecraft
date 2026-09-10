#pragma once

#include "math.hpp"
#include <sys/time.h>

namespace	Packet
{
	struct	Header
	{
		u32	type;
	} __attribute__((packed));

	// type 1
	struct	Position
	{
		Packet::Header	hdr = {.type = 1};

		float			x;
		float			y;
		float			z;
	} __attribute__((packed));

	inline u64	getmsts()
	{
		struct timeval tp;
		gettimeofday(&tp, NULL);
		return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
	}
	// type 2
	struct	RTTRequest
	{
		Packet::Header	hdr = {.type = 2};
	} __attribute__((packed));
	// type 3
	struct	RTTReply
	{
		Packet::Header	hdr = {.type = 3};
	} __attribute__((packed));
};
