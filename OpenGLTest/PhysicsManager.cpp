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
		glm::vec3 pos = object->getPos();
		glm::vec3 v = object->getVelocity();

		glm::vec3 f = netForce(pos);
		glm::vec3 a = f / object->getMass();

		glm::vec3 move = v * dTime + a * dTime * dTime / 2.0f;

		//set new position
		object->setPos(pos + move);

		//set new velocity
		object->setVelocity(v + a * dTime);
	}
}

//Finds the net force on a given point in space
glm::vec3 PhysicsManager::netForce(glm::vec3 pos)
{

}

void PhysicsManager::addObject(PhysicsObject *obj)
{
	objects.push_back(obj);
}