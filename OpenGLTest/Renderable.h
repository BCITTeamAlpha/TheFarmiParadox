#pragma once
//used by Renderer;

#include <vector>

#include <GLEW/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Model {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	std::vector<GLuint> elements;
	GLuint positionLoc;
	GLuint UVLoc;
	GLuint normalLoc;
	GLuint elementLoc;
};

struct Texture {
	std::vector<GLubyte> data;
	int width;
	int height;
	GLuint loc;
};

class Renderable {
	public:
		Renderable();
		glm::vec3 getPosition3();

		Model model;
		Texture texture;
		bool invalidated;

		// transform
		glm::vec2 position;
		float z;
		glm::vec3 rotation;
		glm::vec3 scale;

		// material
		glm::vec4 color;
		bool fullBright;
		float roughness;
		float metallic;
		float f0;
};
