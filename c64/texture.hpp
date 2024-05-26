#pragma once

#include <glad/glad.h>

#include "entity.hpp"

struct Texture : Entity
{
	GLuint texture_id;
	const char* name;
};

unsigned load_texture_to_open_gl_and_get_id(const char* path);
void load_texture_and_add_as_entity(const char* path, const char* name);

int get_texture_format(int nr_channels);
