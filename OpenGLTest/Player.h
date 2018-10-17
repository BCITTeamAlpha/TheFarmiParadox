#pragma once
//created by Game; has an Inventory, Characters
#include "Character.h"
#include "Inventory.h"

class Player {
public:
	Player();
	void addCharacter(Character *c);
	void addWeapon(Weapon w);
private:
	std::vector<Character> chars;
	Inventory *weaps;
};