#pragma once
#include "Character.h"
#include "Bulleto.h"
#include "AssetLoader.h"
#include "EventManager.h"
#include "PhysicsManager.h"
#include "PlayerManager.h"
#include "TextComponent.h"
#include <iostream>
#include <vector>

//This is a class that manages bullet projectiles 
class BulletoManager {
public:

	BulletoManager(PlayerManager *playerManager, PhysicsManager *physics);
	void UpdateBullet();
	void SpawnBulleto(float speedScalar);
	void SetInfoText(std::string info);
	TextComponent *infoText;

private:
	PhysicsManager *physics;
	PlayerManager *playerManager;
	std::vector<Bulleto*> bulletList;		//vector of current bullets
};