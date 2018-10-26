#pragma once

#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Renderable.h"

class AssetLoader {
public:
	// loads a model with supported ASSIMP extensions from file, stores the resulting models in model vector, and returns the first model
	static Model loadModel(std::string const &path);
private:
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	static void processNode(aiNode *node, const aiScene *scene);
	static Model processMesh(aiMesh * mesh);
	static std::vector<Model> models;
};