#pragma once
#include "PhysicsObject.h"


//This is a class to represent bullets
class Bulleto : public PhysicsObject {
public:
	Bulleto(float damage=20);
	bool colliding_with_player(glm::vec2 & player_pos);
private:
	float damage;
};