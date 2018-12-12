#include "MainScene.h"
#include "EventManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "AssetLoader.h"
#include "UIManager.h"

void MainScene::InitScene() {
    UIComponent *mainUI = UIManager::GetComponentById("MainScene");
    if (mainUI != nullptr)
        mainUI->visible = true;

    _playerManager = new PlayerManager();

    // setup Map IRenderable
    _planets.push_back(Planetoid(52.0f, 120.0f, 16.0f));
    _planets.push_back(Planetoid(92.0f, 82.0f, 8.0f));
    _planets.push_back(Planetoid(36.0f, 64.0f, 32.0f));
    _planets.push_back(Planetoid(12.0f, 152.0f, 12.0f));
    _planets.push_back(Planetoid(124.0f, 152.0f, 48.0f));
    _planets.push_back(Planetoid(132.0f, 32.0f, 32.0f));

    _map = new Map(_planets, 175, 205);

    glm::vec3 cameraPos = {_map->width() / 2.0f,
                           _map->height() / 2.0f,
                           _map->height() / 2.0f};
    TypeParam<glm::vec3> param(cameraPos);
    EventManager::notify(RENDERER_SET_CAMERA, &param);


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

    _physics = new PhysicsManager(&_planets, _map);
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
    _models.push_back("../Models/Cow.obj");
    _models.push_back("../Models/Pug.obj");
    _models.push_back("../Models/Slime.obj");

    //create players
    int teams = 4;
    int characters_per_team = 2;

    for (int i = 0; i < teams; i++) {
        for (int j = 0; j < characters_per_team; j++) {
            //set up a square test character
            Character *c = new Character(1000, 10, 10); //health, bullets, maxBullets per turn (recharged when it's their turn again) -> right click to shoot
            c->mass = 50;
            c->controllable = true;
            c->radius = 2.5f;
            c->teamID = i; //i is the current teamid being set - note this is to prevent friendly fire from daniel's bulleto code
            c->set_position(_physics->genSpawnPos(c->radius));

            Renderable *cSkin = new Renderable();
            cSkin->z = 0;
            cSkin->model = AssetLoader::loadModel(_models[j % _models.size()]);
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
            _playerManager->AddPlayer(player);

            // send physicsobjects to physicsmanager
            _physics->addObject(c);
            EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(c->renderable), false);
        }
    }

    // send Renderables to renderer
    EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(_map->renderable), false);
    EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(_background->renderable), false);

    _map->explosion(Planetoid(89, 117, 8));

    _bulletoManager = new HackjobBulletManager(_playerManager, _physics, _map); //initializes bullet manager
	_pickupManager = new PickupManager(_playerManager, _physics);
}

void MainScene::Update(const float delta) {
    _physics->calcPhysics(delta);
    _bulletoManager->UpdateBullet(); //updates hackjob bullets 
	_pickupManager->updatePickup(); //checks pickup collisions
    _playerManager->handlePlayers(delta);
    //_playerManager->UpdatePlayerUI();
}

void MainScene::CleanUp() {
    UIComponent *mainUI = UIManager::GetComponentById("MainScene");
    if (mainUI != nullptr)
        mainUI->visible = false;

    delete _bulletoManager;
    delete _physics;
    delete _background;
    delete _map;
    delete _playerManager;
}