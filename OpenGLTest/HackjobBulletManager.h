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
	HackjobBulletManager(PhysicsManager *physics, Map *map); //the projectile needs information about the map and physics manager
	~HackjobBulletManager();
  void UpdateBullet(float dTime);	//update loop for bullets
	void SpawnBulleto(float speedScalar,int damage, float explodeRadius); //spawns a single projectile
	void SetInfoText(std::string info); //updates ui
	void CheckIfPlayersDamaged(); //checks if any of the players have been damaged
  void notify(EventName eventName, Param* params);    // Overrides ISubscriber::notify
	float waitTime = 10;
private:
	PhysicsManager *physics;
	PlayerManager *playerManager;
	Map *map;
	float timeSince;
	std::vector<HackjobBullet*> bulletList;		//vector of current bullets
};