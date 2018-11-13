#include "AssetLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

std::vector<Model> AssetLoader::models;
std::map<std::string, Texture> AssetLoader::textures;

Model AssetLoader::loadModel(std::string const &path) {
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		throw "Division by zero condition!";
		// return Model();
	}

	int index = models.size();
	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
	// return first mesh read from file
	return models[index];
}

Texture AssetLoader::loadTexture(std::string const & path) {
	if (textures.find(path) == textures.end()) {
		Texture texture;
		stbi_set_flip_vertically_on_load(true);
		GLubyte* texData = stbi_load(path.c_str(), &texture.width, &texture.height, NULL, 4);
		texture.data.assign(texData, texData + texture.width * texture.height * 4);
		textures[path] = texture;
	}

	return textures[path];
}

void AssetLoader::processNode(aiNode *node, const aiScene *scene) {
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		models.push_back(processMesh(mesh));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Model AssetLoader::processMesh(aiMesh *mesh) {
	Model ret;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		glm::vec3 pos;
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		ret.positions.push_back(pos);

		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		ret.normals.push_back(normal);

		if (mesh->mTextureCoords[0]) {
			glm::vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			ret.UVs.push_back(texCoord);
		}
		else {
			ret.UVs.push_back(glm::vec2(0, 0));
		}

	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			ret.elements.push_back(face.mIndices[j]);
		}
	}
	return ret;
}