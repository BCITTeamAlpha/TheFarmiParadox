#pragma once
#include "PhysicsObject.h"
#include "EventManager.h"

class Character : public PhysicsObject {
public:
	Character(int health = 1000, int startingAmmo = 10, int maxBulletsPerTurn = 10);
	void TakeDamage(int amount);
	bool controllable;
	bool left_input;
	bool right_input;
	bool jump_input;
	int playerID;
	int characterID;
	int health;
	int bulletoAmmo;
	int maxBulletsPerTurn;
};