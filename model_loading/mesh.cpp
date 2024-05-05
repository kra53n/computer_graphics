#include "mesh.hpp"

#include <string>

Mesh::Mesh(
	std::vector<Mesh::Vertex> verts,
	std::vector<GLuint> indices,
	std::vector<Mesh::Texture> textures
) {
	this->verts = verts;
	this->indices = indices;
	this->textures = textures;

	setup();
}

void Mesh::setup()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Mesh::Vertex), &verts[0], GL_STATIC_DRAW);	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	int i = 0;
	// vertex pos
	glEnableVertexAttribArray(i);	
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(++i);	
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(++i);	
	glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, tex_coords));
	// vertex tangent
	glEnableVertexAttribArray(++i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(++i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, bi_tangent));
}

void Mesh::draw(ShaderProgram* sp)
{
	GLuint diffuse_nr = 1;
	GLuint specular_nr = 1;
	GLuint normal_nr = 1;
	GLuint height_nr = 1;
	for(int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuse_nr++);
		else if (name == "texture_specular")
			number = std::to_string(specular_nr++);
		else if (name == "texture_normal")
			number = std::to_string(normal_nr++);
		else if (name == "texture_height")
			number = std::to_string(height_nr++);

		sp->set((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLuint)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
