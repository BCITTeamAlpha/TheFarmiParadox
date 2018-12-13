#include "PhysicsObject.h"

PhysicsObject::PhysicsObject() {
	mass = 1.0f;
	radius = 1.0f;
	velocity = glm::vec2(0.0f);
}

bool PhysicsObject::colliding_with_object(PhysicsObject & other, glm::vec2 & collision_normal) {
	collision_normal = this->get_position() - other.get_position();
	float length = glm::length(collision_normal);
	collision_normal /= length;

	return length < (this->radius + other.radius);
}

size_t PhysicsObject::colliding_with_map(Map & map, glm::vec2 pos, glm::vec2 acc, glm::vec2 & collision_normal) {
	glm::vec2 position_of_object_base = pos + this->radius * glm::normalize(acc);
	int floor_x = floor(position_of_object_base.x);
	int floor_y = floor(position_of_object_base.y);

	size_t colliding = 0u;
	collision_normal = glm::vec2(0.0f);

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			if (map.value(floor_x + x, floor_y + y) <= 0) {
				collision_normal.x += 0.5f - x;
				collision_normal.y += 0.5f - y;
				colliding++;
			}
		}
	}

	collision_normal = glm::normalize(collision_normal);

	return colliding;
}