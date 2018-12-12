#pragma once
#include "PhysicsObject.h"


//This is a class to represent bullets
class HackjobBullet : public PhysicsObject {

public:

	HackjobBullet(int damage=100.0f, float explosionRadius = 5.0f);
	bool colliding_with_player(glm::vec2 & player_pos);
	int shooter_PlayerID;
	int bulletAliveTicks = 0;
	int bulletMaxAliveTicks = 300; //deletes bulletes after this many game updates
	int damage;
	float explosionRadius;

private:

};