#pragma once

#include "Scene.h"
#include "PlayerManager.h"
#include "Map.h"
#include "Core.h"
#include "PhysicsManager.h"
#include "HackjobBulletManager.h"
#include "PickupManager.h"
#include <vector>

class MainScene : public Scene {
public:
    void InitScene();
    void Update(const float delta);
    void CleanUp();
private:
    PlayerManager *_playerManager;
    Map *_map;
    PhysicsManager *_physics;
    HackjobBulletManager *_bulletoManager;
	PickupManager *_pickupManager;
    GameObject *_background;

    std::vector<Planetoid> _planets;
	std::vector<Core> _cores;
    std::vector<std::string> _models;
};

