#pragma once
#include "PhysicsObject.h"
#include "Weapon.h"

class Pickup : public PhysicsObject {
public:
	Pickup();
	Pickup(int health);
	Pickup(Weapon* weapon);
	Weapon* pickedUp();
	bool colliding_with_player(glm::vec2 & player_pos);
private:
	Weapon* _weapon;
	int _health;
};