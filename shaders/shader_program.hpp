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
};