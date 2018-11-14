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

	if (currentSlot == -1)
	{
		std::cout << "No weapons available" << std::endl;
		return NULL;
	}

	if (slot < 0)
		currentSlot = _slots.size() - 1;
	else if (slot >= _slots.size())
		currentSlot = 0;
	else
		currentSlot = slot;

	Weapon *w = _slots[currentSlot];

	std::cout << "Name: " << w->_name << ", Charges: " << w->_charges << ", Damage: " << w->_damage << std::endl;

	return _slots[currentSlot];
}

// Removes an item from the Inventory, making a Pickup
Pickup Inventory::dropSlot() {

	// Creates a Pickup for current selected slot
	Pickup drop = Pickup(_slots[currentSlot]);

	// Deletes current selected slot from Inventory
	_slots.erase(_slots.begin());
	
	return drop;
}

// Deletes an item in the Inventory if charges are empty
void Inventory::emptySlot() {
	/*for (std::vector<Weapon*>::size_type i = 0; i != _slots.size(); i++) {
		if (_slots[i]->_charges == 0) {
			_slots.erase(_slots.begin() + i);
		}
	}*/

	_slots.erase(_slots.begin() + currentSlot);
	
	if (_slots.size() == 0)
		currentSlot = -1;
	else
		currentSlot = 0;
}

// Adds a picked up Item if there is room
void Inventory::addItem(Pickup item) {

	// Checks if Inventory full (Capacity set at 5)
	if (_slots.size() < capacity) {
		_slots.push_back(item.pickedUp());		
	}
}

// Removes a charge from the current Item
void Inventory::useWeapon(glm::vec2 pos)
{
	if (currentSlot == -1)
	{
		std::cout << "No weapons available" << std::endl;
		return;
	}

	std::cout << "Remaining Charges: " << _slots[currentSlot]->_charges - 1 << std::endl;

	if (_slots[currentSlot]->use(pos, angle) < 1)
		emptySlot();
}

void Inventory::angleWeapon(float dAngle)
{
	angle += dAngle;

	std::cout << "Weapon Angle = " << angle / 3.14 * 180 << " degrees" << std::endl;

	if (angle >= 6.28)
		angle -= 6.28;
	else if (angle <= -6.28)
		angle += 6.28;
}