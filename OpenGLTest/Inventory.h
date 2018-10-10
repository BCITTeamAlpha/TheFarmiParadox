#pragma once
#include <iostream>
#include "GameObject.h"
#include "Weapon.h"
#include "Pickup.h"
//created by Player

class Inventory : public GameObject {
public:
	Inventory();
	Inventory(std::vector<Weapon> slots);
	std::vector<Weapon> _slots;
	Weapon selectSlot();
	void emptySlot();
	void addItem(Pickup item);

private:
	Weapon currentSlot;
};