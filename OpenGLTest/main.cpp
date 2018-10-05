#include <vector>
#include <Windows.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "MarchingSquares.h"
#include "Renderer.h"
#include "Planetoid.h"
#include "Map.h"
#include "Character.h"
#include "PhysicsManager.h"
#include "Model.h"

IRenderable empty;
IRenderable *p = &empty;
IRenderable ** const pp = &p;

Renderer *renderer;
PhysicsManager *physics;
Map *map;


void SendToRenderer(IRenderable &renderable)
{
	while (*pp != NULL) {
		Sleep(1);
	}
	*pp = &renderable;
	std::cout << "passed IRenderable to Renderer" << std::endl;
}

std::vector<glm::vec3> quadVertices = { { 0, 5, 0 },{ 5, 5, 0 },{ 0, 0, 0 },{ 5, 0, 0 } };
std::vector<glm::vec4> quadColors = { { 1, 0, 0, 1 },{ 1, 0, 0, 1 },{ 1, 0, 0, 1 },{ 1, 0, 0, 1 } };
std::vector<glm::vec3> quadNormals = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
std::vector<GLuint> quadElements = { 1, 0, 2, 1, 2, 3 };

int main()
{
	Model model = Model("teapot.obj");
	// start Renderer in own thread
	renderer = new Renderer(pp);

	// setup Map IRenderable
	std::vector<Planetoid> planets;
	planets.push_back(Planetoid(48.0f, 88.0f, 16.0f));
	planets.push_back(Planetoid(88.0f, 50.0f, 8.0f));
	planets.push_back(Planetoid(32.0f, 32.0f, 32.0f));
	planets.push_back(Planetoid(8.0f, 120.0f, 12.0f));
	planets.push_back(Planetoid(120.0f, 120.0f, 48.0f));
	planets.push_back(Planetoid(128.0f, 0.0f, 32.0f));

	map = new Map(planets, 128, 128);
	map->_vertices = MarchingSquares::GenerateMesh(*map);
	map->_position = { 0, 0, 0 };
	for (GLuint i = 0; i < map->_vertices.size(); i++)
	{
		map->_colors.push_back({ 0, 1, 0, 1 });
		map->_normals.push_back(glm::vec3(0, 0, 1));
		map->_elements.push_back(i);
	}

	physics = new PhysicsManager(&planets, map);

	//set up a square test character
	Character *c = new Character();
	c->_vertices = quadVertices;
	c->_colors = quadColors;
	c->_normals = quadNormals;
	c->_elements = quadElements;
	c->_position = { 64.0f, 32.0f, 0.0f };

	// send IRenderables to renderer
	SendToRenderer(*map);
	SendToRenderer(*c);

	// send physicsobjects to physicsmanager
	physics->addObject(c);

	for (int tick = 0;; tick++)
	{
		physics->calcPhysics();

		Sleep(1000 / 59.94);
	}
}