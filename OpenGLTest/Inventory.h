#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "GameObject.h"
#include "Weapon.h"
#include "Pickup.h"
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

private:
	const int capacity = 5;
};