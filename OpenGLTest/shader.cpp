#include "shader.h"

#include <fstream>
#include <iostream>

Shader::Shader(const std::string& fileName, unsigned int type) {
	if (type == GL_VERTEX_SHADER)
		myShader = CreateShader(LoadShader(fileName + ".vsh"), GL_VERTEX_SHADER);
	else
		myShader = CreateShader(LoadShader(fileName + ".psh"), GL_FRAGMENT_SHADER);
}

Shader::~Shader() {
	glDeleteShader(myShader);
}

std::string Shader::LoadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	} else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type) {
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* srcStrings[1]; //this is an array simply because that's what glShaderSource wants
	GLint lengths[1]; //same with this
	srcStrings[0] = text.c_str();
	lengths[0] = text.length();

	glShaderSource(shader, 1, srcStrings, lengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, "Error compiling shader!");

	return shader;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, const std::string& errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };

	glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE) {
		glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint Shader::GetShader() {
	return myShader;
}