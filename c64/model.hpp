#pragma once

#include <vector>

#include <assimp/scene.h>

#include "mesh.hpp"
#include "shader_program.hpp"

class Model 
{
    std::vector<Mesh::Texture*> _textures;
    std::vector<Mesh> _meshes;
    const char* _path;

public:
    Model(const char* path, const char* model_filename);
    ~Model();
    void draw(ShaderProgram* shader);
    
private:
    void _load_model(const char* filename);
    void _process_node(aiNode* node, const aiScene* scene);
    Mesh _process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh::Texture*> _load_material_textures(
        aiMaterial* mat,
        aiTextureType type,
        const char* type_name,
        Mesh::Vertex::Type* verts_type
    );
    Mesh::Texture* _load_material_texture(
        aiMaterial* mat,
        aiTextureType type,
        const char* type_name,
        Mesh::Vertex::Type* verts_type,
        int idx
    );
};
