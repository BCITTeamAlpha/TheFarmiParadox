#pragma once
//created by Game; has an Inventory, Characters
#include "Character.h"
#include "Inventory.h"
#include "Pickup.h"

class Player {
public:
	Player();
	void addCharacter(Character *c);
	void addItem(Pickup item);
	void prevWeapon();
	void nextWeapon();
	void fireWeapon();
private:
	std::vector<Character> chars;
	Inventory *weaps;
	int curChar;
};