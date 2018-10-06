#pragma once
#include "GameObject.h"

class PhysicsObject : public GameObject {
public:
	PhysicsObject();
	float getMass();
private:
	float mass;
};