#pragma once
//uses UIComponents, Renderables
#include <algorithm>
#include <iostream>
#include <list>
#define _USE_MATH_DEFINES
#include <Math.h>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Renderable.h"
#include "UIRenderable.h"
#include "shader.h"

extern GLFWwindow *window;

class Renderer {
	public:
		Renderer();
		int RenderLoop(Renderable ** pp);
		~Renderer();
	private:
		void draw();

		void GenerateBuffers(Renderable & renderable);
		void PopulateBuffers(Renderable & renderable);
		void AddToRenderables(Renderable & renderable);
		void DrawRenderable(Renderable * renderable);

		void GenerateBuffers(UIRenderable & UIrenderable);
		void PopulateBuffers(UIRenderable & UIrenderable);
		void AddToRenderables(UIRenderable & UIrenderable);
		void DrawUIRenderable(UIRenderable * UIrenderable);

		void CreateShaderProgram(GLuint & programLoc, const char * vertexShaderPath, const char * fragmentShaderPath);

		std::list<Renderable*> renderables;
		std::list<UIRenderable*> UIrenderables;
		const GLuint WIDTH = 1280;
		const GLuint HEIGHT = 720;
		GLuint mainProgram, VAO;
		GLuint uiProgram;
		GLuint mLoc;
		GLuint vLoc;
		GLuint pLoc;
		GLuint u_colorLoc;
		GLuint lightPosLoc;
		glm::vec3 cameraPosition = { 63.5, 63.5, 63.5 };
		GLfloat cameraFOV = 90.0f;
		GLfloat nearClip = 0.1f;
		GLfloat farClip = 100.0f;

		GLuint mLocUI;
		GLuint vpLocUI;
		GLuint u_colorLocUI;
};

