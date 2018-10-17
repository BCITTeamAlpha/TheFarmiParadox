#pragma once
//created by Game; has an Inventory, Characters
#include "Character.h"
#include "Inventory.h"

class Player {
public:
	Player();
private:
	std::vector<Character> chars;
	Inventory weaps;
};