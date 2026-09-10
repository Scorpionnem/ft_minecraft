#pragma once

#include "math.hpp"

namespace	Packet
{
	struct	Header
	{
		u32	id;
	} __attribute__((packed));

	struct	Position
	{
		Packet::Header	hdr;

		float			x;
		float			y;
		float			z;
	} __attribute__((packed));
};
