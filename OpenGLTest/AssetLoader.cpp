#include "AssetLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

AssetLoader::AssetLoader()
{
}

void AssetLoader::loadModel(std::string const &path) {
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) { // if is Not Zero
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void AssetLoader::processNode(aiNode *node, const aiScene *scene) {
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		renderables.push_back(processMesh(mesh));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Renderable AssetLoader::processMesh(aiMesh *mesh) {
	Renderable ret;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		glm::vec3 pos;
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		ret._positions.push_back(pos);

		if (mesh->mNormals != NULL) {
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			ret._normals.push_back(normal);
		}
		else {
			ret._normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		}

		if (mesh->mTextureCoords[0]) {
			glm::vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			ret._texCoords.push_back(texCoord);
		}
		else {
			ret._texCoords.push_back(glm::vec2(0, 0));
		}

	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			ret._elements.push_back(face.mIndices[j]);
		}
	}
	return ret;
}