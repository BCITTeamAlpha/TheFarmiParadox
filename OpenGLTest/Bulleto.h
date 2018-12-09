#pragma once
#include "PhysicsObject.h"


//This is a class to represent bullets
class Bulleto : public PhysicsObject {
public:
	Bulleto(int damage=100.0f);
	bool colliding_with_player(glm::vec2 & player_pos);
	int shooter_PlayerID;
	int shooter_teamID;
	int bulletAliveTicks = 0;
	int bulletMaxAliveTicks = 300; //deletes bulletes after this many game updates
	int damage;
private:
};