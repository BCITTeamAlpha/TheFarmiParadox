#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(std::vector<Planetoid> *p, Map *m)
{
	map = m;
	planets = p;
	objects = std::vector<PhysicsObject*>();
}

void PhysicsManager::calcPhysics()
{
	float x, y;

	for (int i = 0; i < objects.size(); i++)
	{
		//do physics on each object
		PhysicsObject *object = objects.at(i);
		glm::vec2 pos = object->getPos();
		object->setPos(glm::vec2(pos.x + 1, pos.y + 0));
		x = object->getPos().x;
		y = object->getPos().y;
	}
}

void PhysicsManager::addObject(PhysicsObject *obj)
{
	objects.push_back(obj);
}