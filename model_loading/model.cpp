#include "model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "texture.hpp"

Model::Model(const char* path, const char* filename)
{
	_path = path;
	_load_model(filename);
}

Model::~Model()
{
	for (Mesh::Texture* texture : _textures) delete texture;
}

void Model::draw(ShaderProgram* sp)
{
	for(unsigned int i = 0; i < _meshes.size(); i++)
		_meshes[i].draw(sp);
}

void Model::_load_model(const char* filename)
{
	Assimp::Importer importer;
	char buf[256];
	strcpy(buf, _path);
	strcat(buf, filename);
	const aiScene* scene = importer.ReadFile(
		buf,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
	);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("ERROR::ASSIMP::%s\n", importer.GetErrorString());
		return;
	}
	_process_node(scene->mRootNode, scene);
}

void Model::_process_node(aiNode* node, const aiScene* scene)
{
	for(unsigned i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(_process_mesh(mesh, scene));
	}
	for(unsigned i = 0; i < node->mNumChildren; i++)
	{
		_process_node(node->mChildren[i], scene);
	}

}

Mesh Model::_process_mesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Mesh::Vertex> verts;
	std::vector<unsigned> indices;
	std::vector<Mesh::Texture*> textures;

	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x; 
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = vec;

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bi_tangent = vector;
		}
		else
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);

		verts.push_back(vertex);
	}
	for(unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);        
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

	std::vector<Mesh::Texture*> diffuse_maps = _load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

	std::vector<Mesh::Texture*> specular_maps = _load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

	std::vector<Mesh::Texture*> normal_maps = _load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

	std::vector<Mesh::Texture*> height_maps = _load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), height_maps.begin(), height_maps.end());

	return Mesh(verts, indices, textures);
}

std::vector<Mesh::Texture*> Model::_load_material_textures(
	aiMaterial* mat,
	aiTextureType type,
	const char* type_name
) {
	std::vector<Mesh::Texture*> textures;
	for (unsigned i = 0; i < mat->GetTextureCount(type); i++)
	{
		textures.push_back(_load_material_texture(mat, type, type_name, i));
	}
	return textures;
}

Mesh::Texture* Model::_load_material_texture(
	aiMaterial* mat,
	aiTextureType type,
	const char* type_name,
	int idx
) {
	Mesh::Texture* texture = new Mesh::Texture;
	aiString str;
	mat->GetTexture(type, idx, &str);

	char* c_str_copy;
	for (unsigned i = 0; i < _textures.size(); i++)
		if (strcmp(_textures[i]->path, str.C_Str()) == 0)
			return _textures[i];

	char buf[256];
	strcpy(buf, _path);
	strcat(buf, str.C_Str());
	texture->id = load_texture_to_open_gl_and_get_id(buf);
	texture->type = type_name;
	c_str_copy = new char[128];
	strcpy(c_str_copy, str.C_Str());
	texture->path = c_str_copy;
	_textures.push_back(texture);
	return texture;
}
