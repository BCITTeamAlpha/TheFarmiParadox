#include "Inventory.h"

Inventory::Inventory() {

}

Inventory::Inventory(std::vector<Weapon> slots) {
	_slots = slots;
	currentSlot = slots.begin;
}

Weapon Inventory::selectSlot() {
	// Selects Weapon in Inventory for use
	return currentSlot;
}

void Inventory::emptySlot() {
	// Deletes an item in the Inventory
	for (std::vector<Weapon>::size_type i = 0; i != _slots.size(); i++) {
		if (_slots[i]._charges == 0) {
			_slots.erase(_slots.begin() + i);
		}
	}
}

void Inventory::addItem(Pickup item) {
	// Adds an Item when picked up
	_slots.push_back(item.pickedUp);
}   