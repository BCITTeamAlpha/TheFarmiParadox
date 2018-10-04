#pragma once
//Instantiated in the game class, uses the event system
#include "Map.h"
#include "PhysicsObject.h"

class PhysicsManager {
public:
	PhysicsManager(std::vector<Planetoid> *p, Map *m);
	void PhysicsManager::calcPhysics();
	void addObject(PhysicsObject *obj);
private:
	std::vector<Planetoid> *planets;
	std::vector<PhysicsObject> *objects;
	Map *map;
};