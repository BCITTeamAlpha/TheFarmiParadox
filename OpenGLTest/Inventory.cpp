#include "Inventory.h"

Inventory::Inventory() {
	_slots = std::vector<Weapon*>();
	currentSlot = NULL;
}

//Selects Weapon in Inventory for use
Weapon* Inventory::selectSlot(int slot) {

	// Rotate current Inventory slot to the left
	//std::rotate(_slots.begin(), _slots.begin() + 1, _slots.end());

	// Rotate current Inventory slot to the right
	//std::rotate(_slots.rbegin(), _slots.rbegin() + 1, _slots.rend());

	if (slot < 0)
		currentSlot = _slots.size() - 1;
	else if (slot >= _slots.size())
		currentSlot = 0;
	else
		currentSlot = slot;

	return _slots.at(currentSlot);
}

// Removes an item from the Inventory, making a Pickup
Pickup Inventory::dropSlot() {

	// Creates a Pickup for current selected slot
	Pickup drop = Pickup(_slots.at(currentSlot));

	// Deletes current selected slot from Inventory
	_slots.erase(_slots.begin());
	
	return drop;
}

// Deletes an item in the Inventory if charges are empty
void Inventory::emptySlot() {
	for (std::vector<Weapon*>::size_type i = 0; i != _slots.size(); i++) {
		if (_slots[i]->_charges == 0) {
			_slots.erase(_slots.begin() + i);
		}
	}
}

// Adds a picked up Item if there is room
void Inventory::addItem(Pickup item) {

	// Checks if Inventory full (Capacity set at 5)
	if (_slots.size() < capacity) {
		_slots.push_back(item.pickedUp());		
	}
}   