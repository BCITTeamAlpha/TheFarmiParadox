#include "MainScene.h"
#include "EventManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "AssetLoader.h"
#include "UIManager.h"

MainScene::MainScene(int numPlayers, int *models) 
    : _numPlayers(numPlayers), _modelNums(models) { }

void MainScene::InitScene() {
	srand(time(NULL));
    UIComponent *mainUI = UIManager::GetComponentById("MainScene");
    if (mainUI != nullptr)
        mainUI->visible = true;

    _playerManager = new PlayerManager();

    // setup Map IRenderable
    _planets.push_back(Planetoid(147.0f, 120.0f, 16.0f));
    _planets.push_back(Planetoid(188.0f, 82.0f, 8.0f));
    _planets.push_back(Planetoid(131.0f, 64.0f, 32.0f));
    _planets.push_back(Planetoid(107.0f, 152.0f, 12.0f));
    _planets.push_back(Planetoid(219.0f, 152.0f, 48.0f));
    _planets.push_back(Planetoid(227.0f, 32.0f, 32.0f));

    _map = new Map(_planets, 365, 205);

    glm::vec3 cameraPos = {_map->width() / 2.0f,
                           _map->height() / 2.0f,
                           _map->height() / 2.0f};
    TypeParam<glm::vec3> param(cameraPos);
    EventManager::notify(RENDERER_SET_CAMERA, &param);

	// setup cores
	for (int i = 0; i < _planets.size(); i++) {
		_cores.push_back(Core(_planets[i]));
		EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(_cores[i].renderable), false);
	}

    // setup background
    Renderable* backgroundSkin = new Renderable();
    _background = new GameObject();
    _background->setRenderable(backgroundSkin);
    backgroundSkin->z = -10;
    backgroundSkin->position = glm::vec2(_map->width() / 2.0f, _map->height() / 2.0f);
    backgroundSkin->scale.y = (cameraPos.z - backgroundSkin->z) * 2;
    backgroundSkin->scale.x = backgroundSkin->scale.y * (float)_map->width() / (float)_map->height();
    backgroundSkin->model = AssetLoader::loadModel("quad.obj");
    backgroundSkin->texture.width = _map->width();
    backgroundSkin->texture.height = _map->height();
    GLubyte* backgroundImage = new GLubyte[backgroundSkin->texture.width * backgroundSkin->texture.height * 4];
    for (int x = 0; x < backgroundSkin->texture.width; x++) {
        for (int y = 0; y < backgroundSkin->texture.height; y++) {
            size_t index = (x + y * backgroundSkin->texture.width) * 4;
            float val = _map->value(x, y) * 0.1f;
            val = std::max<float>(0.0f, val);
            val = std::min<float>(1.0f, val);
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

    _physics = new PhysicsManager(&_planets, &_cores, _map);
	/*
    for (int i = 0; i < 5; ++i) {
        Pickup *p = new Pickup();
        p->mass = 75;
        p->radius = 2.5f;
        p->set_position(_physics->genSpawnPos(p->radius));

        Renderable *pSkin = new Renderable();
        pSkin->z = 0;
        pSkin->model = AssetLoader::loadModel("../Models/cube.obj");
        pSkin->color = glm::vec4(0.8f, 0.6f, 0.4f, 1.0f);
        p->setRenderable(pSkin);
        pSkin->scale = glm::vec3(5.0f);
        _physics->addObject(p);

        EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(p->renderable), false);
    }
	*/

    _models.push_back("../Models/Cat.obj");
    _models.push_back("../Models/Chick.obj");
    _models.push_back("../Models/Cow.obj");
    _models.push_back("../Models/Dolphin.obj");
    _models.push_back("../Models/Dragon.obj");
    _models.push_back("../Models/Horse.obj");
    _models.push_back("../Models/Llama.obj");
    _models.push_back("../Models/Pig.obj");
    _models.push_back("../Models/Dog.obj");
    _models.push_back("../Models/Pug.obj");
    _models.push_back("../Models/Red Fox.obj");
    _models.push_back("../Models/Sheep.obj");
    _models.push_back("../Models/Whale.obj");
    _models.push_back("../Models/Wolf.obj");
    _models.push_back("../Models/Slime.obj");
    _models.push_back("../Models/Zebra.obj");

    //create players
    int charactersPerPlayer = 12 / _numPlayers;
    _playerManager->charPerPlayer = charactersPerPlayer;
	int playerID = 1;
	int characterID = 1;

    glm::vec4 playerColors[4][2] = {
        {{0.447, 0.098, 0.745, 1.0}, {0.271, 0.024, 0.482, 1.0}},
        {{0.922, 0.373, 0, 1.0}, {0.714, 0.29, 0, 1.0}},
        {{0.047, 0.714, 0.604, 1.0}, {0.0, 0.451, 0.376, 1.0}},
        {{0.922, 0.878, 0, 1.0}, {0.714, 0.682, 0.0, 1.0}},
    };

    UIComponent *component;

    for (int i = 0; i < _numPlayers; i++) {
		//set up a player 
		Player *player = new Player();
		player->playerID = playerID++;

        player->color = playerColors[i][0];
        player->accent = playerColors[i][1];

        component = UIManager::GetComponentById("p" + std::to_string(player->playerID) + "Container");
        if (component != nullptr)
            component->visible = true;

		//set up a test pickup to give the player weapons
		Pickup pickup1 = Pickup(new Weapon("Pistol", 8, 100, 4, 40));
		Pickup pickup2 = Pickup(new Weapon("Grenade", 1, 100, 16, 30));

		player->addItem(pickup1);
		player->addItem(pickup2);

        for (int j = 0; j < charactersPerPlayer; j++) {
            //set up a square test character
            Character *c = new Character(1000, 10, 10); //health, bullets, maxBullets per turn (recharged when it's their turn again) -> right click to shoot
            c->mass = 50;
			if(j==0)
				c->controllable = true; //sets only the first character to controllable initially
            c->radius = 2.5f;
			c->characterID = characterID++;
			c->set_position(_physics->genSpawnPos(c->radius));

            Renderable *cSkin = new Renderable();
            cSkin->z = 0;
            cSkin->model = AssetLoader::loadModel(_models[_modelNums[i]]);
//            float hue = i / (float)_numPlayers + j / ((float)_numPlayers * (float)charactersPerPlayer * 3);
 //           hue = hue * 2.0 * M_PI;
            cSkin->color = player->color;
//            cSkin->color = glm::vec4(std::sin(hue) * 0.5f + 0.5f, std::sin(hue + 2) * 0.5f + 0.5f, std::sin(hue + 4) * 0.5f + 0.5f, 1);
            cSkin->scale = glm::vec3(2.5f);
            c->setRenderable(cSkin);

            c->playerID = player->playerID;
            player->addCharacter(c);

            // send physicsobjects to physicsmanager
            _physics->addObject(c);
            EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(c->renderable), false);

            component = UIManager::GetComponentById("c" + std::to_string(j + 1) + "Container");
            if (component != nullptr)
                component->visible = true;
        }
		_playerManager->AddPlayer(player);
    }

	_aimIndicator = new GameObject();
	Renderable *aimSkin = new Renderable();
	aimSkin->model = AssetLoader::loadModel("../Models/cube.obj");
	aimSkin->scale = glm::vec3(2.0);
	aimSkin->color = glm::vec4(1.0, 0.25, 0.0, 1.0);
	aimSkin->fullBright = true;
	_aimIndicator->setRenderable(aimSkin);
	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(_aimIndicator->renderable), false);

    // send Renderables to renderer
    EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(_map->renderable), false);
    EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(_background->renderable), false);

    _map->explosion(Planetoid(184, 117, 8));

    _bulletoManager = new HackjobBulletManager(_physics, _map); //initializes bullet manager
	_pickupManager = new PickupManager(_playerManager, _physics);
}

void MainScene::Update(const float delta) {
    _physics->calcPhysics(delta);
    _bulletoManager->UpdateBullet(); //updates hackjob bullets 
	_pickupManager->updatePickup(); //checks pickup collisions
    int win = _playerManager->handlePlayers(delta);

	Character* c = _playerManager->GetCurrentPlayer()->getCurrentCharacter();
	glm::vec2 pos = c->get_position();
	glm::vec3 rot = c->get_rotation();
	rot.z += _playerManager->GetCurrentPlayer()->aim_angle;
	rot.z += (rot.y == 0.0f) ? -45 : 45;
	pos.x += -sin(rot.z * (float)M_PI / 180.0f) * 2.0f * c->radius;
	pos.y += cos(rot.z * (float)M_PI / 180.0f) * 2.0f * c->radius;
	_aimIndicator->set_position(pos);

	if (win != -1)
		printf("Player %d wins!!!!!!!!!!!!!!!!", win);

    _playerManager->UpdatePlayerUI();
}

void MainScene::CleanUp() {
    UIComponent *mainUI = UIManager::GetComponentById("MainScene");
    if (mainUI != nullptr)
        mainUI->visible = false;

    delete _bulletoManager;
    delete _physics;
    delete _background;
    delete _aimIndicator;
    delete _map;
    delete _playerManager;
}