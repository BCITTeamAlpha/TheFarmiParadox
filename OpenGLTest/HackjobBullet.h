#pragma once
#include "PhysicsObject.h"


//This is a class to represent bullets
class HackjobBullet : public PhysicsObject {

public:

	HackjobBullet(int damage=100.0f, float explosionRadius = 5.0f);
	bool colliding_with_player(glm::vec2 & player_pos);
	int shooter_PlayerID; //this is set to prevent friendly fire between players
	int shooterCharacterID; //this is to prevent a player's own character from damaging his/her other characters
	int bulletAliveTicks = 0; 
	int bulletMaxAliveTicks = 300; 
	int damage;	//damage is in int, as max health is also int.
	float explosionRadius; //for explosive projectiles, this is to be used in map-> explode based on radius.

private:

};