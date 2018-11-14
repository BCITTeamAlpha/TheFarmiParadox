#pragma once
#include <string>
#include "GameObject.h"
#include <glm/glm.hpp>
//created by Player

class Weapon : public GameObject {
public:
	Weapon();
	Weapon(std::string name, int charges, int damage);
	std::string _name;
	int _charges;
	int _damage;
	int use(glm::vec2 pos, float angle);
};