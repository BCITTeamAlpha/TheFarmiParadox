#pragma once
#include "GameObject.h"
#include "Map.h"

class PhysicsObject : public GameObject {
	public:
	PhysicsObject();
	float mass;
	float radius;
	glm::vec2 velocity;
	bool grounded;

	bool colliding_with_object(PhysicsObject &other, glm::vec2 &collision_normal);
	size_t colliding_with_map(Map & map, glm::vec2 pos, glm::vec2 acc, glm::vec2 &collision_normal);
};