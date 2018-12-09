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
#include "playerManager.h"
#include "BulletoManager.h"

GLFWwindow* window;
std::mutex mtx;
std::condition_variable cv;
Input inputHandler;
//If we want to bind a key directly to a function
//inputHandler.addKeyDownBinding(GLFW_KEY_WHATEVER, Class::func or class.func);
double xpos, ypos;

Renderer *renderer;
PhysicsManager *physics;
PlayerManager *playerManager;
Map *map;
SoundManager* noise;
BulletoManager* bulletoManager;
int Player::PLAYER_COUNT = 0;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_LEFT:
	case GLFW_KEY_A:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);  	
			EventManager::notify(PLAYER_LEFT, &param, false);
			EventManager::notify(AIM_LEFT, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(PLAYER_LEFT, &param, false);
			EventManager::notify(AIM_LEFT, &param, false);
		}
		break;
	case GLFW_KEY_RIGHT:
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);
			EventManager::notify(PLAYER_RIGHT, &param, false);
			EventManager::notify(AIM_RIGHT, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(PLAYER_RIGHT, &param, false);
			EventManager::notify(AIM_RIGHT, &param, false);
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
            
            //SoundParams * JumpNoise = new SoundParams();

            //JumpNoise->sound = Jump;

            //JumpNoise->x = 0;
            //JumpNoise->y = 0;
            //JumpNoise->z = 0;

            //TypeParam<SoundParams*> jumpSound(JumpNoise);
            //EventManager::notify(PLAY_SOUND, &jumpSound);
            noise->playSound(Jump, 0, 0, 0);
		}
		break;
	case GLFW_KEY_F:
		if (action == GLFW_PRESS)
		{
			EventManager::notify(PLAYER_FIRE, nullptr, false);
		}
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
		float bulletSpeedScalar = 42.069f;
		bulletoManager->SpawnBulleto(bulletSpeedScalar);

	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) //GLFW_RELEASE is the other possible state.
	{
		printf("Right mouse button released\n");
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) //GLFW_RELEASE is the other possible state.
	{
		TypeParam<std::pair<float, float>> param(std::pair<float,float>(xpos, ypos));
		EventManager::notify(UI_CLICK, &param, false);
		printf("left mouse button clicked at: ");
		printf("%lf %lf\n", xpos, ypos);
	}
}


int main()
{
	srand(time(NULL));
	// start Renderer in own thread
	renderer = new Renderer();
	std::unique_lock<std::mutex> lck(mtx);
	std::thread renderThread = std::thread(&Renderer::RenderLoop, renderer);

	// setup Map IRenderable
	std::vector<Planetoid> planets;
	planets.push_back(Planetoid(52.0f, 120.0f, 16.0f));
	planets.push_back(Planetoid(92.0f, 82.0f, 8.0f));
	planets.push_back(Planetoid(36.0f, 64.0f, 32.0f));
	planets.push_back(Planetoid(12.0f, 152.0f, 12.0f));
	planets.push_back(Planetoid(124.0f, 152.0f, 48.0f));
	planets.push_back(Planetoid(132.0f, 32.0f, 32.0f));

	map = new Map(planets, 175, 205);

	renderer->cameraFOV = 70;
	renderer->cameraPosition.x = map->width() / 2.0f;
	renderer->cameraPosition.y = map->height() / 2.0f;
	renderer->cameraPosition.z = (map->height() / 2.0f) / std::tan(renderer->cameraFOV * M_PI / 360.0f);

	// setup background
	Renderable* backgroundSkin = new Renderable();
	GameObject background;
	background.setRenderable(backgroundSkin);
	backgroundSkin->z = -10;
	backgroundSkin->position = glm::vec2(map->width()/2.0f, map->height()/2.0f);
	backgroundSkin->scale.y = std::tan(renderer->cameraFOV * M_PI / 360.0f) * (renderer->cameraPosition.z - backgroundSkin->z) * 2;
	backgroundSkin->scale.x = backgroundSkin->scale.y * (float)map->width() / (float)map->height();
	backgroundSkin->model = AssetLoader::loadModel("quad.obj");
	backgroundSkin->texture.width = map->width();
	backgroundSkin->texture.height = map->height();
	GLubyte* backgroundImage = new GLubyte[backgroundSkin->texture.width * backgroundSkin->texture.height * 4];
	for (int x = 0; x < backgroundSkin->texture.width; x++) {
		for (int y = 0; y < backgroundSkin->texture.height; y++) {
			size_t index = (x + y * backgroundSkin->texture.width) * 4;
			float val = map->value(x, y) * 0.1f;
			val = std::max(0.0f, val);
			val = std::min(1.0f, val);
			val = 1.0f - val;
			val = val * val;

			backgroundImage[index] = 63;
			backgroundImage[index + 1] = 127;
			backgroundImage[index + 2] = 255;
			backgroundImage[index + 3] = 255 * val;
			
			if (rand() % 255 == 0 && backgroundImage[index + 3] == 0) {
				backgroundImage[index] = 255 - backgroundImage[index + 3];
				backgroundImage[index + 1] = 255 - backgroundImage[index + 3];
				backgroundImage[index + 2] = 255 - backgroundImage[index + 3];
				backgroundImage[index + 3] = 255;
			}
		}
	}
	backgroundSkin->texture.data.assign((GLubyte*)backgroundImage, (GLubyte*)backgroundImage + backgroundSkin->texture.width * backgroundSkin->texture.height * 4);
	delete backgroundImage;
	backgroundSkin->fullBright = true;

	physics = new PhysicsManager(&planets, map);
	playerManager = new PlayerManager();

	for (int i = 0; i < 5; ++i) {
		Pickup *p = new Pickup();
		p->mass = 75;
		p->radius = 2.5f;
		p->set_position(physics->genSpawnPos(p->radius));

		Renderable *pSkin = new Renderable();
		pSkin->z = 0;
		pSkin->model = AssetLoader::loadModel("../Models/cube.obj");
		pSkin->color = glm::vec4(0.8f, 0.6f, 0.4f, 1.0f);
		p->setRenderable(pSkin);
		pSkin->scale = glm::vec3(5.0f);
		physics->addObject(p);

		EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(p->renderable), false);
	}

	std::vector<std::string> models = std::vector<std::string>();
	models.push_back("../Models/Cat.obj");
	models.push_back("../Models/Cow.obj");
	models.push_back("../Models/Pug.obj");
	models.push_back("../Models/Slime.obj");

	//create players
	int teams = 3;
	int characters_per_team = 2;
	
	for (int i = 0; i < teams; i++) {
		for (int j = 0; j < characters_per_team; j++) {
			//set up a square test character
			Character *c = new Character();
			c->mass = 50;
			c->controllable = true;
			c->radius = 2.5f;
			c->teamID = i; //i is the current teamid being set - note this is to prevent friendly fire from daniel's bulleto code
			c->set_position(physics->genSpawnPos(c->radius));

			Renderable *cSkin = new Renderable();
			cSkin->z = 0;
			cSkin->model = AssetLoader::loadModel(models[j % models.size()]);
			float hue = i / (float)teams + j / ((float)teams * (float)characters_per_team * 3);
			hue = hue * 2.0 * M_PI;
			cSkin->color = glm::vec4(std::sin(hue) * 0.5f + 0.5f, std::sin(hue + 2) * 0.5f + 0.5f, std::sin(hue + 4) * 0.5f + 0.5f, 1);
			cSkin->scale = glm::vec3(2.5f);
			c->setRenderable(cSkin);

			//set up a player with the test character
			Player *player = new Player();

			//set up a test pickup to give the player weapons
			Pickup pickup1 = Pickup(new Weapon("Gun", 5, 20));
			Pickup pickup2 = Pickup(new Weapon("Grenade", 1, 50));

			player->addItem(pickup1);
			player->addItem(pickup2);
				
			c->playerID = player->playerID; //omegalul ultra duplication
			player->addCharacter(c);
			playerManager->AddPlayer(player);

			// send physicsobjects to physicsmanager
			physics->addObject(c);
			EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(c->renderable), false);
		}
	}

	// send Renderables to renderer
	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(map->renderable), false);
	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(background.renderable), false);

	//Set input handling callbacks
	cv.wait(lck);
	inputHandler.setInputCallbacks(window, KeyCallback, mouse_button_callback);

	EventManager::subscribe(PLAYER_LEFT, playerManager);
	EventManager::subscribe(PLAYER_RIGHT, playerManager);
	EventManager::subscribe(PLAYER_JUMP, playerManager);
	EventManager::subscribe(AIM_LEFT, playerManager);
	EventManager::subscribe(AIM_RIGHT, playerManager);
	EventManager::subscribe(PLAYER_FIRE, playerManager);
	
	//TESTING FOR THE INVENTORY/WEAPON SYSTEM
	inputHandler.addKeyDownBinding(GLFW_KEY_Q, PlayerManager::prevWeapon);
	inputHandler.addKeyDownBinding(GLFW_KEY_E, PlayerManager::nextWeapon);

    //adding sound
    noise = new SoundManager();
    EventManager::subscribe(PLAY_SONG, noise);
    EventManager::subscribe(PLAY_SOUND, noise);


    //start initial music track
    TrackParams * initial = new TrackParams();

    initial->track = MenuBGM;

    initial->x = 0;
    initial->y = 0;
    initial->z = 0;

    TypeParam<TrackParams*> param(initial);
    EventManager::notify(PLAY_SONG, &param);

	map->explosion(Planetoid(89, 117, 8));

	bulletoManager = new BulletoManager(playerManager, physics); //initializes bullet manager

	for (int tick = 0;; tick++)
	{
		bulletoManager->UpdateBullet(); //updates bullets 
		physics->calcPhysics(1.0 / 59.94);
		playerManager->handlePlayers(1.0 / 59.94);
		Sleep(1000.0 / 59.94);
	}
}