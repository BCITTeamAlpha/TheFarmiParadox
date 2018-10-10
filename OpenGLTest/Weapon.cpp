#include "Weapon.h"

Weapon::Weapon() {

}

Weapon::Weapon(std::string name, int charges, int damage) {
	_name = name;
	_charges = charges;
	_damage = damage;
}

void Weapon::use() {
	// Uses charge when used by player
	_charges--;
}