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
#include "Player.h"

const int physUpdates = 30;

GLFWwindow* window;
Input inputHandler;
//If we want to bind a key directly to a function
//inputHandler.addKeyDownBinding(GLFW_KEY_WHATEVER, Class::func or class.func);
double xpos, ypos;

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
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);
			EventManager::notify(PLAYER_LEFT, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(PLAYER_LEFT, &param, false);
		}
		break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);
			EventManager::notify(PLAYER_RIGHT, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(PLAYER_RIGHT, &param, false);
		}
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);
			EventManager::notify(PLAYER_JUMP, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(PLAYER_JUMP, &param, false);
		}
		break;
	case GLFW_KEY_Q:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);
			EventManager::notify(WEAPON_PREV, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(WEAPON_PREV, &param, false);
		}
		break;
	case GLFW_KEY_E:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);
			EventManager::notify(WEAPON_NEXT, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(WEAPON_NEXT, &param, false);
		}
		break;
	default:
		if (action == GLFW_PRESS) inputHandler.onKeyPress(key);
		if (action == GLFW_REPEAT) inputHandler.onKeyRepeat(key);

		break;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	glfwGetCursorPos(window, &xpos, &ypos);
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) //GLFW_RELEASE is the other possible state.
	{
		printf("Right mouse button clicked at: ");
		printf("%lf %lf\n", xpos, ypos);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) //GLFW_RELEASE is the other possible state.
	{
		printf("Right mouse button released\n");
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) //GLFW_RELEASE is the other possible state.
	{
		printf("left mouse button clicked at: ");
		printf("%lf %lf\n", xpos, ypos);
	}
}


void TestFunction() {
	std::cout << "TestFunction called" << std::endl;
}


std::vector<glm::vec3> quadPositions = { { -2.5, 2.5, 0 },{ 2.5, 2.5, 0 },{ -2.5, -2.5, 0 },{ 2.5, -2.5, 0 } };
std::vector<glm::vec2> quadTexCoords = { { 0, 1 },{ 1, 1 },{ 0, 0 },{ 1, 0 } };
std::vector<glm::vec3> quadNormals = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
std::vector<GLuint> quadElements = { 1, 0, 2, 1, 2, 3 };
std::vector<glm::vec3> backgroundPositions = { { 0, 128, 0 },{ 128, 128, 0 },{ 0, 0, 0 },{ 128, 0, 0 } };

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
	mapSkin->_z = 0;
	mapSkin->_positions = MarchingSquares::GenerateMesh(*map);
	mapSkin->_color = glm::vec4(0.5, 1, 0, 1);

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
	c->mass = 50;
	c->position = { 75.0f, 60.0f };

	Renderable *cSkin = new Renderable();
	cSkin->_z = 0;
	cSkin->_positions = quadPositions;
	cSkin->_texCoords = quadTexCoords;
	cSkin->_normals = quadNormals;
	cSkin->_elements = quadElements;
	cSkin->_color = glm::vec4(1, 0, 0, 1);

	c->setRenderable(cSkin);

	//set up a player with the test character
	Player *player1 = new Player();

	// setup background
	GLubyte backgroundImage[128][128][4];
	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			float val = map->value(x, y) * 0.1f;
			val = std::max(0.0f, val);
			val = std::min(1.0f, val);
			val = 1.0f - val;
			val = val * val;
			backgroundImage[y][x][0] = 63;
			backgroundImage[y][x][1] = 127;
			backgroundImage[y][x][2] = 255;
			backgroundImage[y][x][3] = 255 * val;
		}
	}
	Renderable* backgroundSkin = new Renderable();
	GameObject background;
	background.setRenderable(backgroundSkin);
	backgroundSkin->_z = -1;
	backgroundSkin->_positions = backgroundPositions;
	backgroundSkin->_texCoords = quadTexCoords;
	backgroundSkin->_normals = quadNormals;
	backgroundSkin->_elements = quadElements;
	backgroundSkin->_texture.assign((GLubyte*)backgroundImage, (GLubyte*)backgroundImage + 128 * 128 * 4);
	backgroundSkin->_fullBright = true;

	// send Renderables to renderer
	SendToRenderer(*mapSkin);
	SendToRenderer(*cSkin);
	SendToRenderer(*backgroundSkin);

	// send physicsobjects to physicsmanager
	physics->addObject(c);

	//Set input handling callbacks
	inputHandler.setInputCallbacks(window, KeyCallback, mouse_button_callback);

	EventManager::subscribe(PLAYER_LEFT, physics); //Subscribe player left to EventManager
	EventManager::subscribe(PLAYER_RIGHT, physics); //Subscribe player right to EventManager
	EventManager::subscribe(PLAYER_JUMP, physics); //Subscribe player jump to EventManager

	for (int tick = 0;; tick++)
	{
		physics->calcPhysics(1.0 / 59.94);
		Sleep(1000.0 / 59.94);
	}
}