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
	int health;
	int playerID; //i know this is omega duplication, but it just sets this playerID to the same as the player holding this character...  too late into the term so yolo no refactoring needed, if crash gg wp
	int teamID; //basically what team you're on... 1, 2, 3.. etc
	int bulletoAmmo;
	int maxBulletsPerTurn;
};