#include "PhysicsManager.h"

const float PhysicsManager::VELOCITY_CAP = 40.0f;

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
		glm::vec2 pos = object->position;
		glm::vec2 v = object->velocity;

		if (object->mass == 0)
		{
			std::cout << "Attempting to divide by zero. Aborting physics." << std::endl;
			std::terminate();
		}

		glm::vec2 a = gravAcceleration(pos);

		glm::vec2 move = v * dTime + a * dTime * dTime / 2.0f;

		//set new position
		object->position = pos + move;

		//cap maximum velocity
		v += a * dTime;
		
		if (glm::length(v) > VELOCITY_CAP)
		{
			v = glm::normalize(v) * VELOCITY_CAP;
		}

		object->velocity = v;
	}
}

//Finds the net force on a given point in space
glm::vec2 PhysicsManager::gravAcceleration(glm::vec2 pos)
{
	glm::vec2 nAccel = glm::vec2(0, 0);
	std::vector<float> accels = std::vector<float>();

	//calculate the force of each planetoid and add them to the vector
	for (Planetoid p : *planets)
	{
		//calculate distance between planet and object
		glm::vec2 dir = p._pos - pos;
		
		float dist = glm::length(dir);
		
		dist = std::max(dist, 10.0f);

		//calculate the force from each planetoid
		float a = p._m / (dist * dist);

		//add force to the total force
		accels.push_back(a);
		nAccel += glm::normalize(dir) * a;
	}

	return nAccel;
}

void PhysicsManager::addObject(PhysicsObject *obj)
{
	objects.push_back(obj);
}