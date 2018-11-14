#include "Weapon.h"

Weapon::Weapon() {

}

Weapon::Weapon(std::string name, int charges, int damage) {
	_name = name;
	_charges = charges;
	_damage = damage;
}

//Displays weapon, uses it, then removes a charge
int Weapon::use(glm::vec2 pos, float angle)
{


	return --_charges;
}