#include "Pickup.h"

float minimumPickupDistance = 1.5f;

Pickup::Pickup() {

}

Pickup::Pickup(Weapon* weapon) {
	_weapon = weapon;
}

Weapon* Pickup::pickedUp() {
	return _weapon;
}


bool Pickup::colliding_with_player(glm::vec2 & player_pos) {
	float length = glm::length(get_position() - player_pos);
	return length < (this->radius + minimumPickupDistance);
}
