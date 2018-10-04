#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(std::vector<Planetoid> *p, Map *m)
{
	map = m;
	planets = p;
}

void PhysicsManager::calcPhysics()
{
	for (int i = 0; i < objects->size(); i++)
	{
		//do physics on each object
	}
}

void PhysicsManager::addObject(PhysicsObject *obj)
{
	objects->push_back(*obj);
}