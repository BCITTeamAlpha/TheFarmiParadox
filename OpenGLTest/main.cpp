#include <vector>
#include <Windows.h>

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <glm/glm.hpp>

#include "AssetLoader.h"
#include "MarchingSquares.h"
#include "Renderer.h"
#include "Renderable.h"
#include "Planetoid.h"
#include "Map.h"
#include "Character.h"
#include "PhysicsManager.h"
#include "Input.h"
#include "UIManager.h"
#include "SoundManager.h"
#include <thread>
#include "Player.h"

GLFWwindow* window;
Input inputHandler;
//If we want to bind a key directly to a function
//inputHandler.addKeyDownBinding(GLFW_KEY_WHATEVER, Class::func or class.func);
double xpos, ypos;

Renderer *renderer;
PhysicsManager *physics;
Map *map;
SoundManager* noise;

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

int main()
{
	// start Renderer in own thread
	renderer = new Renderer();
	std::thread renderThread = std::thread(&Renderer::RenderLoop, renderer);

    //adding sound
    noise= new SoundManager();
    EventManager::subscribe(PLAY_SONG, noise);
    EventManager::subscribe(PLAY_SOUND, noise);


    //start initial music track
    TrackParams initial;
    initial.track = MainBGM;

    initial.position[0] = 0;
    initial.position[1] = 0;
    initial.position[2] = 0;
    EventManager::notify(PLAY_SONG, &TypeParam<TrackParams>(initial));
    

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
	mapSkin->z = 0;
	mapSkin->model = MarchingSquares::GenerateModel(*map);
	mapSkin->texture = AssetLoader::loadTexture("checkerboard.png");
	mapSkin->color = glm::vec4(0.5, 1, 0, 1);

	map->setRenderable(mapSkin);

	physics = new PhysicsManager(&planets, map);

	//set up a square test character
	Character *c = new Character();
	c->mass = 50;
	c->position = { 75.0f, 60.0f };

	Renderable *cSkin = new Renderable();
	cSkin->z = 0;
	cSkin->model = AssetLoader::loadModel("teapot.obj");
	cSkin->color = glm::vec4(1, 0, 0, 1);

	c->setRenderable(cSkin);

	//set up a player with the test character
	Player *player1 = new Player();

	//set up a test pickup to give the player weapons
	Pickup pickup1 = Pickup(new Weapon("Gun", 5, 20));
	Pickup pickup2 = Pickup(new Weapon("Grenade", 1, 50));

	player1->addItem(pickup1);
	player1->addItem(pickup2);

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
	backgroundSkin->z = -1;
	backgroundSkin->position = new glm::vec2(64, 64);
	backgroundSkin->scale = glm::vec3(128, 128, 128);
	backgroundSkin->model = AssetLoader::loadModel("quad.obj");
	backgroundSkin->texture.data.assign((GLubyte*)backgroundImage, (GLubyte*)backgroundImage + 128 * 128 * 4);
    backgroundSkin->texture.width = 128;
    backgroundSkin->texture.height = 128;
    backgroundSkin->fullBright = true;

	// send Renderables to renderer
	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<Renderable*>(mapSkin), false);
	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<Renderable*>(cSkin), false);
	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<Renderable*>(backgroundSkin), false);

	// send physicsobjects to physicsmanager
	physics->addObject(c);

	//Set input handling callbacks
	Sleep(500); // Sleep until the renderer is done initializing. This is a horrible solution.
	inputHandler.setInputCallbacks(window, KeyCallback, mouse_button_callback);

	EventManager::subscribe(PLAYER_LEFT, physics); //Subscribe player left to EventManager
	EventManager::subscribe(PLAYER_RIGHT, physics); //Subscribe player right to EventManager
	EventManager::subscribe(PLAYER_JUMP, physics); //Subscribe player jump to EventManager
	
	//TESTING FOR THE INVENTORY/WEAPON SYSTEM
	inputHandler.addKeyDownBinding(GLFW_KEY_Q, Player::prevWeapon);
	inputHandler.addKeyDownBinding(GLFW_KEY_E, Player::nextWeapon);
	inputHandler.addKeyDownBinding(GLFW_KEY_F, Player::fireWeapon);

	for (int tick = 0;; tick++)
	{
		physics->calcPhysics(1.0 / 59.94);
		Sleep(1000.0 / 59.94);
	}
}