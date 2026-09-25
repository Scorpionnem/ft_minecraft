#pragma once

#include "game/entity/Entity.hpp"

class	Entities
{
	public:
		Entities() {}
		~Entities() {}

		u64	add(Entity en)
		{
			en.id = _id;
			_entities[_id] = en;
			return (_id++);
		}
		void	set(u64 id, Entity en)
		{
			en.id = id;
			_entities[id] = en;
		}
		Entity&	get(u64 id)
		{
			return (_entities[id]);
		}
		void	draw(const mbl::render::Camera& cam)
		{
			for (auto& [id, en] : _entities)
			{
			    mbl::utils::aabb3f	box = {.pos = en.pos - en.size / 2, .size = en.size};
			    mbl::render::renderer::AABBRenderer::draw(box, cam, vec3f(1));
			    mbl::render::renderer::RayRenderer::draw(en.pos + en.eye, en.pos + en.eye + mbl::render::Camera::front(en.yaw, en.pitch), cam, vec3f(0, 0, 1));
			}
		}
		void	clear()
		{
			_entities.clear();
		}
		void	remove(u64 id)
		{
			_entities.erase(id);
		}
	private:
		u64	_id = 1;
		std::map<u64, Entity>	_entities;
};
