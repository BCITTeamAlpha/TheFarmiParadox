#pragma once
//Instantiated in the game class, uses the event system
#include "Map.h"
#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include <iostream>

class PhysicsManager {
public:
	PhysicsManager(std::vector<Planetoid> *p, Map *m);
	void PhysicsManager::calcPhysics(float dTime);
	void addObject(PhysicsObject *obj);
	static const float VELOCITY_CAP;
private:
	void characterMovement(glm::vec2 & pos, glm::vec2 & vel);
	glm::vec2 gravAcceleration(glm::vec2 pos);
	std::vector<Planetoid> *planets;
	std::vector<PhysicsObject*> objects;
	Map *map;
};