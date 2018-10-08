#pragma once
//Instantiated in the game class, uses the event system
#include "Map.h"
#include "PhysicsObject.h"
#include <glm/glm.hpp>

class PhysicsManager {
public:
	PhysicsManager(std::vector<Planetoid> *p, Map *m);
	void PhysicsManager::calcPhysics(float dTime);
	void addObject(PhysicsObject *obj);
private:
	glm::vec2 netForce(glm::vec2 pos, float m);
	std::vector<Planetoid> *planets;
	std::vector<PhysicsObject*> objects;
	Map *map;
};