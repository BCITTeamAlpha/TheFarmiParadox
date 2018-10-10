#include "Pickup.h"

Pickup::Pickup() {

}

Pickup::Pickup(Weapon weapon) {
	_weapon = weapon;
}

Weapon Pickup::pickedUp() {
	// Destroy Pickup and returns its contents
	return _weapon;
}