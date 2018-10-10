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

		if (object->getMass() == 0)
		{
			std::cout << "Attempting to divide by zero. Aborting physics." << std::endl;
			return;
		}

		glm::vec2 a = gravAcceleration(pos);

		glm::vec2 move = v * dTime + a * dTime * dTime / 2.0f;

		//set new position
		object->setPos(pos + move);

		//set new velocity
		object->setVelocity(v + a * dTime);
	}
}

//Finds the net force on a given point in space
glm::vec2 PhysicsManager::gravAcceleration(glm::vec2 pos)
{
	glm::vec2 nAccel = glm::vec2(0, 0);

	//calculate the force of each planetoid and add them to the vector
	for (Planetoid p : *planets)
	{
		//calculate distance between planet and object
		glm::vec2 dir = p._pos - pos;
		
		float dist = glm::length(dir) * p.C_SCALE;
		
		//ensure distance is not zero to prevent dividing by zero
		if (dist == 0.0f)
			dist = 1;

		dir = glm::normalize(dir);

		//calculate the force from each planetoid
		float a = p._m / (dist * dist);

		//add force to the total force
		nAccel += dir * a;
	}

	return nAccel;
}

void PhysicsManager::addObject(PhysicsObject *obj)
{
	objects.push_back(obj);
}