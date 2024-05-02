#pragma once

#include <glad/glad.h>

class Texture
{
	GLuint _ID;
	int _wdt;
	int _hgt;
	int _nr_channels;
	const char* _uniform_name;

public:
	Texture(
		const char* path,
		const char* unfirom_name,
		GLenum wrap_option = GL_REPEAT,
		bool flip = true
	);

	const char* get_uniform_name();
	GLuint ID();
};


int get_texture_format(int nr_channels);
