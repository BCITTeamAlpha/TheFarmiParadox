#pragma once
#include "Character.h"
#include "HackjobBullet.h"
#include "AssetLoader.h"
#include "EventManager.h"
#include "PhysicsManager.h"
#include "PlayerManager.h"
#include "Map.h"
#include <iostream>
#include <vector>

//This is a class that manages bullet projectiles 
class HackjobBulletManager : public ISubscriber {

public:

	HackjobBulletManager(PlayerManager *playerManager, PhysicsManager *physics, Map *map);
	void UpdateBullet();
	void SpawnBulleto(float speedScalar,int damage, float explodeRadius);
	void SetInfoText(std::string info);
	void CheckIfPlayersDamaged();
    void notify(EventName eventName, Param* params);    // Overrides ISubscriber::notify

private:
	PhysicsManager *physics;
	PlayerManager *playerManager;
	Map *map;
	std::vector<HackjobBullet*> bulletList;		//vector of current bullets
};