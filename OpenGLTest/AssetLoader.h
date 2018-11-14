#pragma once
#include <map>
#include <vector>
#include <assimp/scene.h>
#include "Renderable.h"

class AssetLoader {
public:
	// loads a model with supported ASSIMP extensions from file, stores the resulting models in model vector, and returns the first model
	static Model loadModel(std::string const &path);
	static Texture loadTexture(std::string const &path);
private:
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	static void processNode(aiNode *node, const aiScene *scene);
	static Model processMesh(aiMesh * mesh);
	static std::vector<Model> models;
	static std::map<std::string, Texture> textures;
};