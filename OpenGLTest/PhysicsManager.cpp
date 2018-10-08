#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(std::vector<Planetoid> *p, Map *m)
{
	map = m;
	planets = p;
	objects = std::vector<PhysicsObject*>();
}

void PhysicsManager::calcPhysics(float dTime)
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		//do physics on each object
		PhysicsObject *object = objects.at(i);
		glm::vec2 pos = object->getPos();
		glm::vec2 v = object->getVelocity();

		glm::vec2 f = netForce(pos, object->getMass());
		glm::vec2 a = f / object->getMass();

		glm::vec2 move = v * dTime + a * dTime * dTime / 2.0f;

		//set new position
		object->setPos(pos + move);

		//set new velocity
		object->setVelocity(v + a * dTime);
	}
}

//Finds the net force on a given point in space
glm::vec2 PhysicsManager::netForce(glm::vec2 pos, float m)
{
	std::vector<glm::vec2> forces = std::vector<glm::vec2>();

	for (Planetoid p : *planets)
	{
		//calculate the force from each planetoid
		
	}

	return glm::vec2(0, 0);
}

void PhysicsManager::addObject(PhysicsObject *obj)
{
	objects.push_back(obj);
}