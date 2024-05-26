#pragma once

#include <vector>

#include <assimp/scene.h>
#include <glm//glm.hpp>

#include "mesh.hpp"
#include "shader_program.hpp"

class Model 
{
    std::vector<Mesh::MTexture*> _textures;
    unsigned screen_texture_id;
    std::vector<Mesh> _meshes;
    const char* _path;

    glm::mat2 rotation_matrix_for_screen_texture;

public:
    Model(const char* path, const char* model_filename);
    ~Model();
    void draw(ShaderProgram* shader);
    
private:
    void _load_model(const char* filename);
    void _process_node(aiNode* node, const aiScene* scene);
    Mesh _process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh::MTexture*> _load_material_textures(
        aiMaterial* mat,
        aiTextureType type,
        const char* type_name,
        Mesh::Vertex::Type* verts_type
    );
    Mesh::MTexture* _load_material_texture(
        aiMaterial* mat,
        aiTextureType type,
        const char* type_name,
        Mesh::Vertex::Type* verts_type,
        int idx
    );
};
