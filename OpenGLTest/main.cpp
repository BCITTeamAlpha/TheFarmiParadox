#include <vector>
#include <Windows.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "MarchingSquares.h"
#include "Renderer.h"
#include "Renderable.h"
#include "Planetoid.h"
#include "Map.h"
#include "Character.h"
#include "PhysicsManager.h"
#include "Model.h"
#include "Input.h"
#include <thread>

GLFWwindow* window;
Input inputHandler; //usage: inputHandler.addKeyDownBinding(GLFW_KEY_B, yourFunctionName); 

Renderable *p;
Renderable ** const pp = &p;

Renderer *renderer;
PhysicsManager *physics;
Map *map;


void SendToRenderer(Renderable &renderable)
{
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


std::vector<glm::vec3> quadPositions = { { -2.5, 2.5, 0 },{ 2.5, 2.5, 0 },{ -2.5, -2.5, 0 },{ 2.5, -2.5, 0 } };
std::vector<glm::vec2> quadTexCoords = { { 0, 1 },{ 1, 1 },{ 0, 0 },{ 1, 0 } };
std::vector<glm::vec3> quadNormals = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
std::vector<GLuint> quadElements = { 1, 0, 2, 1, 2, 3 };

int main()
{
	Model model = Model("teapot.obj");
	// start Renderer in own thread
	renderer = new Renderer();
	std::thread renderThread = std::thread(&Renderer::RenderLoop, renderer, pp);

	// setup Map IRenderable
	std::vector<Planetoid> planets;
	planets.push_back(Planetoid(48.0f, 88.0f, 16.0f));
	planets.push_back(Planetoid(88.0f, 50.0f, 8.0f));
	planets.push_back(Planetoid(32.0f, 32.0f, 32.0f));
	planets.push_back(Planetoid(8.0f, 120.0f, 12.0f));
	planets.push_back(Planetoid(120.0f, 120.0f, 48.0f));
	planets.push_back(Planetoid(128.0f, 0.0f, 32.0f));

	map = new Map(planets, 128, 128);

	Renderable *mapSkin = new Renderable();
	mapSkin->_positions = MarchingSquares::GenerateMesh(*map);
	mapSkin->_color = glm::vec4(0, 1, 0, 1);

	for (GLuint i = 0; i < mapSkin->_positions.size(); i++)
	{
		glm::vec2 texCoord;
		texCoord.x = mapSkin->_positions[i].x / 128;
		texCoord.y = mapSkin->_positions[i].y / 128;
		mapSkin->_texCoords.push_back(texCoord);
		mapSkin->_normals.push_back(glm::vec3(0, 0, 1));
		mapSkin->_elements.push_back(i);
	}

	map->setRenderable(mapSkin);

	physics = new PhysicsManager(&planets, map);

	//set up a square test character
	Character *c = new Character();
	c->setPos({ 64.0f, 32.0f, 0.0f });

	Renderable *cSkin = new Renderable();
	cSkin->_positions = quadPositions;
	cSkin->_texCoords = quadTexCoords;
	cSkin->_normals = quadNormals;
	cSkin->_elements = quadElements;
	cSkin->_color = glm::vec4(1, 0, 0, 1);

	c->setRenderable(cSkin);

	// send Renderables to renderer
	SendToRenderer(*mapSkin);
	SendToRenderer(*cSkin);

	// send physicsobjects to physicsmanager
	physics->addObject(c);

	//Set input handling callbacks
	glfwSetKeyCallback(window, KeyCallback);
	inputHandler.addKeyDownBinding(GLFW_KEY_Q, TestFunction); //example of registering a function to input handler. this function will be called whenever Q is tapped 

	for (int tick = 0;; tick++) {
		physics->calcPhysics();
		(*cSkin->_rotation).z += 1.0f;
		Sleep(1000.0f / 59.94f);
	}
}