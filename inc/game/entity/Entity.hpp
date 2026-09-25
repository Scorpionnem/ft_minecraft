#pragma once

#include "mbl.hpp"

struct	Entity
{
	u64		id = 0;

	vec3f	size;
	vec3f	eye; // pos + eye = actual eye pos
	vec3f	pos;
	float	yaw = 0;
	float	pitch = 0;
};

struct	Player : public Entity
{
	Player() : Entity() {
		size = vec3f(0.8, 0.8, 0.8);
	}
};
