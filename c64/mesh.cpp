#include "mesh.hpp"

#include <string>

#include "entity.hpp"
#include "texture.hpp"

Mesh::Mesh(
	std::vector<Mesh::Vertex> verts,
	Vertex::Type verts_type,
	std::vector<GLuint> indices,
	std::vector<Mesh::MTexture*> textures
) {
	this->verts = verts;
	this->verts_type = verts_type;
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
	int i;
	for(i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i]->type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuse_nr++);
		else if (name == "texture_specular")
			number = std::to_string(specular_nr++);
		else if (name == "texture_normal")
			number = std::to_string(normal_nr++);
		else if (name == "texture_height")
			number = std::to_string(height_nr++);

		sp->set((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->id);
	}

	sp->set("verts_type", (int)verts_type);
	if (verts_type == Mesh::Vertex::Type::Screen)
	{
		std::unordered_map<GLuint, Entity*>* textures = get_entities(Entity::Group::Texture);
		int screen_texture_id = 0;
		for (auto it : *textures)
		{
			Texture* texture = (Texture*)it.second;
			if (strcmp(texture->name, "goku") == 0)
			{
				screen_texture_id = texture->texture_id;
				glActiveTexture(GL_TEXTURE0 + i);
				sp->set(("texture_diffuse" + std::to_string(diffuse_nr)).c_str(), i++);
				glBindTexture(GL_TEXTURE_2D, screen_texture_id);
				break;
			}
		}
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLuint)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
