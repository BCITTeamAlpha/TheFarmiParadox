#pragma once
//used by Renderer; has Models
#include <vector>

#include <GLEW/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

class Model {
	public:
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> UVs;
		std::vector<GLuint> elements;
		GLuint positionLoc;
		GLuint UVLoc;
		GLuint normalLoc;
		GLuint elementLoc;
};

class Texture {
	public:
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

		glm::vec2 *position;
		float z;
		glm::vec3 *rotation;
		glm::vec3 scale = glm::vec3(1.0);
		glm::vec4 color = glm::vec4(1.0); // initialize color to white
		bool fullBright;
		const GLuint id; // use ID for signalling renderer to remove from rendering list?
		// or maybe have a setable flag that tells Renderer to delete it?
	private:
		static GLuint count; // monotically incrementing variable used to setting renderable IDs 
};
