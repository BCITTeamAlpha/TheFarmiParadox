#pragma once
#include "GameObject.h"

class PhysicsObject : public GameObject {
public:
	PhysicsObject();
	float getMass();
	glm::vec3 getVelocity();
	void setVelocity(glm::vec3 v);
private:
	float mass;
	glm::vec3 velocity;
};