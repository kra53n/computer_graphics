#pragma once

#include <unordered_map>

struct Entity
{
	enum class Group
	{
		Light,
		Another,
	};

	unsigned id;
	Group group;
};

extern std::unordered_map<unsigned, Entity*> entities;
extern std::unordered_map<Entity::Group, std::unordered_map<unsigned, Entity*>> entity_groups;

unsigned get_new_entity_id();
void add_entity(Entity::Group group, Entity* entity);
void del_entity(unsigned id);
Entity* get_entity(unsigned id);
std::unordered_map<unsigned, Entity*>* get_entities(Entity::Group group);
