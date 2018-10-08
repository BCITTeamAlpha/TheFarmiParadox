#pragma once
#include "GameObject.h"

class PhysicsObject : public GameObject {
public:
	PhysicsObject();
	float getMass();
	glm::vec2 getVelocity();
	void setVelocity(glm::vec2 v);
private:
	float mass;
	glm::vec2 velocity;
};