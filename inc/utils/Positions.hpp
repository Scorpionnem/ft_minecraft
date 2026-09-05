#pragma once

#include "math.hpp"

enum class Facing
{
	POS_X,
	NEG_X,
	POS_Y,
	NEG_Y,
	POS_Z,
	NEG_Z,
	INVALID,
};

enum class FacingCardinal
{
	NORTH = static_cast<int>(Facing::NEG_Z),
	SOUTH = static_cast<int>(Facing::POS_Z),
	EAST = static_cast<int>(Facing::POS_X),
	WEST = static_cast<int>(Facing::NEG_X),
	UP = static_cast<int>(Facing::POS_Y),
	DOWN = static_cast<int>(Facing::NEG_Y),
	INVALID = static_cast<int>(Facing::INVALID),
};

inline Facing	facing(vec3f dir)
{
	float	abs_x = std::abs(dir.x());
	float	abs_y = std::abs(dir.y());
	float	abs_z = std::abs(dir.z());

	if (abs_x > abs_y && abs_x > abs_z)
		return (dir.x() >= 0 ? Facing::POS_X : Facing::NEG_X);
	if (abs_y > abs_x && abs_y > abs_z)
		return (dir.y() >= 0 ? Facing::POS_Y : Facing::NEG_Y);
	if (abs_z > abs_y && abs_z > abs_x)
		return (dir.z() >= 0 ? Facing::POS_Z : Facing::NEG_Z);
	return (Facing::INVALID);
}

namespace std
{
	inline std::string	to_string(FacingCardinal f)
	{
		switch (f)
		{
			case FacingCardinal::UP:
				return ("up");
			case FacingCardinal::DOWN:
				return ("down");
			case FacingCardinal::EAST:
				return ("east");
			case FacingCardinal::WEST:
				return ("west");
			case FacingCardinal::SOUTH:
				return ("south");
			case FacingCardinal::NORTH:
				return ("north");
			default:
				return ("invalid");
		}
	}
	inline std::string	to_string(Facing f)
	{
		switch (f)
		{
			case Facing::POS_X:
				return ("+X");
			case Facing::NEG_X:
				return ("-X");
			case Facing::POS_Y:
				return ("+Y");
			case Facing::NEG_Y:
				return ("-Y");
			case Facing::POS_Z:
				return ("+Z");
			case Facing::NEG_Z:
				return ("-Z");
			default:
				return ("invalid");
		}
	}
};
