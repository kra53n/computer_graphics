#pragma once

#include <vector>

#include <glad/glad.h>

#include "shader.hpp"

class ShaderProgram
{
	GLuint _ID;
	std::vector<Shader> _shaders;

	void _link_program();
	void _compile();
public:

	ShaderProgram(std::vector<Shader> shaders);
	~ShaderProgram();
	void use();

	void set(const char* uniform_name, bool val);
	void set(const char* inform_name, int val);
	void set(const char* uniform_name, float val);
	void set(const char* uniform_name, const std::vector<float>& vals);
};