#include "Weapon.h"

Weapon::Weapon() {

}

Weapon::Weapon(std::string name, int charges, int damage, float explosionRad, float speed) {
	_name = name;
	_charges = charges;
	_damage = damage;
	_explosionRad = explosionRad;
	_speed = speed;
}

//Displays weapon, uses it, then removes a charge
int Weapon::use()
{
	return --_charges;
}