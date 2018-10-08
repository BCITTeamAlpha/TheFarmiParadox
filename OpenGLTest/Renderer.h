#pragma once
//uses UIComponents, Renderables
#include <algorithm>
#include <iostream>
#include <list>
#define _USE_MATH_DEFINES
#include <Math.h>
#include <thread>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderable.h"
#include "shader.h"

extern GLFWwindow *window;

class Renderer {
	public:
		Renderer();
		int RenderLoop(Renderable ** pp);
		~Renderer();
	private:
		void DrawRenderable(Renderable * renderable);
		void draw();
		void GenerateBuffers(Renderable & renderable);
		void PopulateBuffers(Renderable & renderable);
		void AddToRenderables(Renderable & renderable);
		void RemoveFromRenderables(Renderable & renderable);

		std::list<Renderable*> renderables;
		const GLint WIDTH = 1280;
		const GLint HEIGHT = 720;
		GLuint mainProgram, VAO;
		GLuint mLoc;
		GLuint vLoc;
		GLuint pLoc;
		GLuint lightPosLoc;
		glm::vec3 cameraPosition = { 63.5, 63.5, 63.5 };
		float cameraFOV = 90.0f;
		float nearClip = 0.1f;
		float farClip = 100.0f;
};

