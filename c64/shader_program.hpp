#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "shader.hpp"

class ShaderProgram
{
	GLuint _ID;
	std::vector<Shader> _shaders;
	std::vector<Texture> _textures;

	void _link_program();
	void _compile();

public:
	ShaderProgram(std::vector<Shader> shaders);
	~ShaderProgram();

	GLuint ID();
	void use();
	void del();
	void recompile();

	void set(const char* uniform_name, bool val);
	void set(const char* inform_name, int val);
	void set(const char* uniform_name, float val);
	void set(const char* uniform_name, const glm::vec3& val);
	void set(const char* uniform_name, const glm::mat2& val);
	void set(const char* uniform_name, const glm::mat4& val);
};