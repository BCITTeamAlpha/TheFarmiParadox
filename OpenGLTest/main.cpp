#include <vector>
#include <Windows.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "MarchingSquares.h"
#include "Renderer.h"
#include "Planetoid.h"
#include "Map.h"
#include "Model.h"
#include "Input.h"

IRenderable empty;
IRenderable *p = &empty;
IRenderable ** const pp = &p;
GLFWwindow* window;
Input inputHandler; //usage: inputHandler.addKeyDownBinding(GLFW_KEY_B, yourFunctionName); 

void SendToRenderer(IRenderable &renderable) {
	while (*pp != NULL) {
		Sleep(1);
	}
	*pp = &renderable;
	std::cout << "passed IRenderable to Renderer" << std::endl;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_W:
		if (action == GLFW_RELEASE)
		{
			std::cout << "W key released" << std::endl;
		}
		if (action == GLFW_PRESS)
		{
			std::cout << "W key pressed" << std::endl;
		}
		if (action == GLFW_REPEAT)
		{
			std::cout << "W key REPEATED" << std::endl;
		}
		break;
	case GLFW_KEY_A:
		break;

	default:

		if (action == GLFW_PRESS) inputHandler.onKeyPress(key);
		if (action == GLFW_REPEAT) inputHandler.onKeyRepeat(key);

		break;
	}
}

void TestFunction() {
	std::cout << "TestFunction called" << std::endl;
}


std::vector<glm::vec3> quadVertices = { { 0, 5, 0 },{ 5, 5, 0 },{ 0, 0, 0 },{ 5, 0, 0 } };
std::vector<glm::vec4> quadColors = { { 1, 0, 0, 1 },{ 1, 0, 0, 1 },{ 1, 0, 0, 1 },{ 1, 0, 0, 1 } };
std::vector<glm::vec3> quadNormals = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
std::vector<GLuint> quadElements = { 1, 0, 2, 1, 2, 3 };

int main() {
	Model model = Model("teapot.obj");
	// start Renderer in own thread
	Renderer renderer = Renderer(pp);


	// setup Map IRenderable
	std::vector<Planetoid> planets;
	planets.push_back(Planetoid(48.0f, 88.0f, 16.0f));
	planets.push_back(Planetoid(88.0f, 50.0f, 8.0f));
	planets.push_back(Planetoid(32.0f, 32.0f, 32.0f));
	planets.push_back(Planetoid(8.0f, 120.0f, 12.0f));
	planets.push_back(Planetoid(120.0f, 120.0f, 48.0f));
	planets.push_back(Planetoid(128.0f, 0.0f, 32.0f));
	Map map = Map(planets, 128, 128);
	map._vertices = MarchingSquares::GenerateMesh(map);
	map._position = { 0, 0, 0 };
	for (GLuint i = 0; i < map._vertices.size(); i++) {
		map._colors.push_back({ 0, 1, 0, 1 });
		map._normals.push_back(glm::vec3(0, 0, 1));
		map._elements.push_back(i);
	}

	// setup quad IRenderable
	IRenderable quad;
	quad._vertices = model.renderables[0]._vertices;
	quad._colors = model.renderables[0]._colors;
	quad._normals = model.renderables[0]._normals;
	quad._elements = model.renderables[0]._elements;
	quad._position = { 64.0f, 32.0f, 0.0f };

	// setup another quad IRenderable
	IRenderable quad1;
	quad1._vertices = quadVertices;
	quad1._colors = quadColors;
	quad1._normals = quadNormals;
	quad1._elements = quadElements;
	quad1._position = { 64.0f, 32.0f, 0.0f };

	// send IRenderables to renderer
	SendToRenderer(map);
	SendToRenderer(quad);
	SendToRenderer(quad1);


	//Set input handling callbacks
	glfwSetKeyCallback(window, KeyCallback);
	inputHandler.addKeyDownBinding(GLFW_KEY_Q, TestFunction); //example of registering a function to input handler. this function will be called whenever Q is tapped 

	for (int tick = 0;; tick++) {
		//
		// Colt, put your stuff somewhere around here for now
		//
		quad1._position.x += 0.05f;
		if (quad1._position.x > 125.5f) {
			quad1._position.x = -2.5f;
		}
		quad1._position.y += 0.1f;
		if (quad1._position.y > 125.5f) {
			quad1._position.y = -2.5f;
		}
		quad._position.x = quad1._position.x + 16*sin(tick * 0.01f);
		quad._position.y = quad1._position.y + 16*cos(tick * 0.01f);

		Sleep(1000 / 59.94);
	}
}