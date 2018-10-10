#pragma once
#include "GameObject.h"

class PhysicsObject : public GameObject {
public:
	PhysicsObject();
	float mass;
	glm::vec2 velocity;
private:
};