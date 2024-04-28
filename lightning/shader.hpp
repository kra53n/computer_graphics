#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>

class Shader
{
	GLuint _ID;
	const char* _path;

	GLenum _get_shader_type();
	const char* _get_shader_type_as_str();
	std::string _read_file();
	void _compile();

public:
	Shader(const char* shader_path);
	~Shader();
	void attach(GLuint shader_program_id);
	void recompile();
	void del();
};