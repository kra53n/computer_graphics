#include "entity.hpp"

std::unordered_map<unsigned, Entity*> entities;
std::unordered_map<Entity::Group, std::unordered_map<unsigned, Entity*>> entity_groups;

unsigned get_new_entity_id()
{
	static unsigned id = 0;
	return id++;
}

void add_entity(Entity::Group group, Entity* entity)
{
	unsigned id = get_new_entity_id();
	entity->id = id;
	entities.insert({ id, entity });
	entity_groups[group].insert({ id, entity });
}

void del_entity(unsigned id)
{
	Entity* entity = entities[id];
	entities.erase(id);
	entity_groups[entity->group].erase(id);
	delete entity;
}

Entity* get_entity(unsigned id)
{
	auto iter = entities.find(id);
	if (iter == entities.end())
		return nullptr;
	return iter->second;
}

std::unordered_map<unsigned, Entity*>* get_entities(Entity::Group group)
{
	auto iter = entity_groups.find(group);
	if (iter == entity_groups.end())
		return nullptr;
	return &entity_groups[group];
}