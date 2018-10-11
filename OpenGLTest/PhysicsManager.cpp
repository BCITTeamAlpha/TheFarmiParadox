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
	//do physics on each object
	for (size_t i = 0; i < objects.size(); i++)
	{
		PhysicsObject *object = objects[i];

		if (object->mass == 0)
		{
			std::cout << "Attempting to divide by zero. Aborting physics." << std::endl;
			std::terminate();
		}

		glm::vec2 pos = object->position;
		glm::vec2 vel = object->velocity;
		glm::vec2 acc = gravAcceleration(pos);
		glm::vec2 move = vel * dTime + acc * dTime * dTime / 2.0f;

		//cap maximum movement
		if (length(move) / dTime > VELOCITY_CAP) 
		{
			move = glm::normalize(move) * VELOCITY_CAP;
		}

		pos += move;

		vel += acc * dTime;

		//cap maximum velocity
		if (glm::length(vel) > VELOCITY_CAP) 
		{
			vel = glm::normalize(vel) * VELOCITY_CAP;
		}

		object->position = pos;
		object->velocity = vel;
		object->rotation = glm::vec3(0, 0, std::atan2(acc.y, acc.x) * 180.0f / M_PI);
	}

	// takes by reference and modifies character's pos and vel
	characterMovement(objects[0]);
}

void PhysicsManager::characterMovement(PhysicsObject *object) {
	float player_speed = 10.0f;
	float player_radius = 2.5f;
	float player_jump_speed = 10.0f;
	float input_xAxis = -1.0f;
	float input_jump = 1.0f;

	glm::vec2 pos = object->position;
	glm::vec2 vel = object->velocity;

	for (Planetoid planet : *planets) {
		glm::vec2 planet_to_obj = pos - planet._pos;
		GLfloat len = glm::length(planet_to_obj) - player_radius;
		if (len < planet._r) {
			planet_to_obj /= len;
			pos = planet._pos + planet._r * planet_to_obj;
			if (glm::dot(vel, planet_to_obj) < 0.0f) {
				vel = input_xAxis * player_speed * glm::vec2(planet_to_obj.y, -planet_to_obj.x);
			}
			vel += input_jump * player_jump_speed * planet_to_obj;
			break;
		}
	}

	object->position = pos;
	object->velocity = vel;
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