#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "GameObject.h"
#include "Weapon.h"
#include "Pickup.h"
#include <glm/glm.hpp>
//created by Player

class Inventory : public GameObject {
public:
	Inventory();
	std::vector<Weapon*> _slots;
	int currentSlot;

	Weapon* selectSlot(int slot);
	Pickup dropSlot();
	void emptySlot();
	void addItem(Pickup item);
	void useWeapon(glm::vec2 pos);
	float angle;
private:
	const int capacity = 5;
};