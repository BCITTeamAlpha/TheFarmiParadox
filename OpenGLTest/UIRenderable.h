#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

class UIRenderable {
	public:
		UIRenderable();
		void BuildWithString(std::string text);
		glm::vec3 *_position;
		glm::vec3 *_rotation;
		glm::vec4 _color = glm::vec4(1.0); // initialize color to white
		std::vector<glm::vec2> _positions;
		std::vector<glm::vec2> _texCoords;
		std::vector<GLuint> _elements;
		std::vector<GLubyte> _texture;
		GLuint _positionBufferLocation;
		GLuint _texCoordBufferLocation;
		GLuint _elementBufferLocation;
		GLuint _textureLocation;
		const GLuint _id; // use ID for signalling renderer to remove from rendering list?
						  // or maybe have a setable flag that tells Renderer to delete it?
	private:
		static GLuint count; // monotically incrementing variable used to setting renderable IDs 
		glm::vec2 charToVec2(char c);
};