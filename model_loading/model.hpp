#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "shader_program.hpp"

#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model 
{
    std::vector<Mesh::Texture> _textures;
    std::vector<Mesh> _meshes;
    const char* _path;

public:
    Model(const char* path, const char* model_filename);
    void draw(ShaderProgram* shader);
    
private:
    void _load_model(const char* filename);
    void _process_node(aiNode* node, const aiScene* scene);
    Mesh _process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh::Texture> _load_material_textures(aiMaterial* mat, aiTextureType type, const char* type_name);
    Mesh::Texture _load_material_texture(aiMaterial* mat, aiTextureType type, const char* type_name, int idx);
};
