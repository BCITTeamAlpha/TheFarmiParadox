#include "PhysicsObject.h"

PhysicsObject::PhysicsObject() {
	mass = 1.0f;
	radius = 1.0f;
	velocity = glm::vec2(0.0f);
}

bool PhysicsObject::colliding_with_object(PhysicsObject & other, glm::vec2 & collision_normal) {
	collision_normal = this->position - other.position;
	float length = glm::length(collision_normal);
	collision_normal /= length;

	return length < (this->radius + other.radius);
}

bool PhysicsObject::colliding_with_map(Map & map, glm::vec2 acc, glm::vec2 & collision_normal) {
	glm::vec2 normalized_acceleration = glm::normalize(acc);
	glm::vec2 position_of_object_base = this->position + this->radius * normalized_acceleration;
	int floor_x = floor(position_of_object_base.x);
	int floor_y = floor(position_of_object_base.y);

	bool colliding = false;
	collision_normal = glm::vec2(0);

	for (size_t x = 0; x < 2; x++) {
		for (size_t y = 0; y < 2; y++) {
			if (map.value(floor_x + x, floor_y + y) <= 0) {
				collision_normal.x += 0.5 - x;
				collision_normal.y += 0.5 - y;
				colliding = true;
			}
		}
	}

	if (collision_normal != glm::vec2(0)) {
		collision_normal = glm::normalize(collision_normal);
	} else {
		collision_normal = -normalized_acceleration;
	}

	return colliding;
}