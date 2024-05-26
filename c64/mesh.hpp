#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader_program.hpp"

class Mesh
{
public:
	struct Vertex
	{
		enum class Type
		{
			Other = 0,
			Screen = 1,
		};

		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 tex_coords;
		glm::vec3 tangent;
		glm::vec3 bi_tangent;
	};

	struct MTexture
	{
		GLuint id;
		const char* type;
		const char* path;
	};

	std::vector<Vertex> verts;
	Vertex::Type verts_type;
	std::vector<GLuint> indices;
	std::vector<MTexture*> textures;

	GLuint VAO;

	Mesh(
		std::vector<Vertex> verts,
		Vertex::Type verts_type,
		std::vector<GLuint> indices,
		std::vector<MTexture*> textures
	);

	void draw(ShaderProgram* sp);

private:
	GLuint _VBO;
	GLuint _EBO;

	void setup();
};
