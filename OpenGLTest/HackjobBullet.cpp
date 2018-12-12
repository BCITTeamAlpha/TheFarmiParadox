#include "HackjobBullet.h"

float minimumCollisionDistance = 3.5f;


HackjobBullet::HackjobBullet(int damage, float explosionRadius) {
	this->damage = damage;
	this->explosionRadius = explosionRadius;
}


bool HackjobBullet::colliding_with_player(glm::vec2 & player_pos) {
	float length = glm::length(get_position() - player_pos);
	return length < (this->radius + minimumCollisionDistance);
}