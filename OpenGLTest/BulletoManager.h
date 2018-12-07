#pragma once
#include "Character.h"
#include "Bulleto.h"
#include "AssetLoader.h"
#include "EventManager.h"
#include "PhysicsManager.h"
#include <iostream>
#include <vector>

//This is a class that manages bullet projectiles 
class BulletoManager {
public:

	BulletoManager(std::vector<Character*> playerList, std::vector<Bulleto*> bulletList, PhysicsManager *physics);
	void UpdateBullet();
	void SpawnBulleto(int x, int y);

private:
	std::vector<Character*> playerListPtr;
	std::vector<Bulleto*> bulletListPtr;
	PhysicsManager *physicsManagerPointer;
	float tempSpeed = 0.0001f;
	float bulletDamage = 20.0f;
};