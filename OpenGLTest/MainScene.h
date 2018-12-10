#pragma once

#include "Scene.h"
#include "PlayerManager.h"
#include "Map.h"
#include "PhysicsManager.h"
#include "HackjobBulletManager.h"
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
    GameObject *_background;

    std::vector<Planetoid> _planets;
    std::vector<std::string> _models;
};

