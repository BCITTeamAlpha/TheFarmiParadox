#pragma once
#include "GameObject.h"

class PhysicsObject : public GameObject {
public:
	PhysicsObject();
	float getMass();
	void setMass(float m);
	glm::vec2 getVelocity();
	void setVelocity(glm::vec2 v);
private:
	float mass;
	glm::vec2 velocity;
};