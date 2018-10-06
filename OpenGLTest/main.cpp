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

Renderable empty;
Renderable *p = &empty;
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

	Renderable *mapSkin = new Renderable();
	mapSkin->_vertices = MarchingSquares::GenerateMesh(*map);

	for (GLuint i = 0; i < mapSkin->_vertices.size(); i++)
	{
		mapSkin->_colors.push_back({ 0, 1, 0, 1 });
		mapSkin->_normals.push_back(glm::vec3(0, 0, 1));
		mapSkin->_elements.push_back(i);
	}

	map->setRenderable(mapSkin);

	physics = new PhysicsManager(&planets, map);

	//set up a square test character
	Character *c = new Character();
	c->setPos({ 64.0f, 32.0f, 0.0f });

	Renderable *cSkin = new Renderable();
	cSkin->_vertices = quadVertices;
	cSkin->_colors = quadColors;
	cSkin->_normals = quadNormals;
	cSkin->_elements = quadElements;

	c->setRenderable(cSkin);

	// send IRenderables to renderer
	SendToRenderer(*mapSkin);
	SendToRenderer(*cSkin);

	// send physicsobjects to physicsmanager
	physics->addObject(c);

	for (int tick = 0;; tick++)
	{
		physics->calcPhysics();

		Sleep(1000 / 59.94);
	}
}