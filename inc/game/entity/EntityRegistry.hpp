#pragma once

#include "game/entity/Entity.hpp"

#include <unordered_map>

class	EntityRegistry
{
	public:
		EntityRegistry() {}
		~EntityRegistry() {}

		void	clear()
		{
			_entities.clear();
			_currentId = 0;
		}
		void	remove(u64 id)
		{
			_entities.erase(id);
		}
		Entity*	insert(u64 id, Entity entity)
		{
			if (_entities.contains(id))
				return (get(id));
			entity.id = id;
			_entities[id] = entity;
			return (&_entities[id]);
		}
		Entity*	add(Entity entity)
		{
			entity.id = _currentId;
			_entities[_currentId] = entity;
			return (get(_currentId++));
		}
		Entity*	get(u64 id)
		{
			auto	find = _entities.find(id);
			if (find != _entities.end())
				return (&find->second);
			return (nullptr);
		}
		const std::unordered_map<u64, Entity>&	get_all()
		{
			return (_entities);
		}
	private:
		std::unordered_map<u64, Entity>	_entities;
		u64								_currentId = 0;
};
