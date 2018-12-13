#include "HackjobBullet.h"

float minimumCollisionDistance = 3.5f;

//Constructor to set up a projectile with a damage value as well as explosion radius
HackjobBullet::HackjobBullet(int damage, float explosionRadius) {
	this->damage = damage;
	this->explosionRadius = explosionRadius;
}

//Checks if a bullet is within the minimumCollisionDistance of a player.
bool HackjobBullet::colliding_with_player(glm::vec2 & player_pos) {
	float length = glm::length(get_position() - player_pos);
	return length < (this->radius + minimumCollisionDistance);
}