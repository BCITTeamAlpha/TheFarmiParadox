#include "PhysicsManager.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

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
		object->rotation.z = atan2(acc.x, -acc.y) * 180.0f / M_PI + 90.0f;
		auto *character = dynamic_cast<Character *>(object);
		if (character)
		{
			characterMovement(character);
		}
	}

	// takes by reference and modifies character's pos and vel
	characterMovement(objects[0]);
}

void PhysicsManager::characterMovement(PhysicsObject *object) {
	float player_speed = 10.0f;
	float player_radius = 2.5f;
	float player_jump_speed = 20.0f;

	glm::vec2 pos = object->position;
	glm::vec2 vel = object->velocity;

	// loop over planets, check if character is inside any of them them
	for (Planetoid planet : *planets) {
		glm::vec2 planet_to_obj = pos - planet._pos;
		GLfloat len = glm::length(planet_to_obj) - player_radius;
		// if character close to planet
		if (len - 1 < planet._r) {
			// allign character with surface
			object->rotation.z = atan2(-planet_to_obj.x, planet_to_obj.y) * 180.0f / M_PI + 90.0f;
			// if character is inside planet
			if (len < planet._r) {
				planet_to_obj /= len;
				// push character out of planet
				pos = planet._pos + planet._r * planet_to_obj;
				// set velocity to 0 if still moving towards planet
				if (glm::dot(vel, planet_to_obj) < 0.0f) {
					vel = glm::vec2(0);
				}
				// horizontal movement
				vel += (-player_left_input + player_right_input) * player_speed * glm::vec2(planet_to_obj.y, -planet_to_obj.x);
				// jumping
				vel += player_jump_input * player_jump_speed * planet_to_obj;
				break;
			}
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


void PhysicsManager::notify(EventName eventName, Param* param) {
	switch (eventName) {
		case PLAYER_LEFT: {
			TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(param); // Safetly cast generic param pointer to a specific type
			if (p != nullptr) this->player_left_input = p->Param;
			break;
		}
		case PLAYER_RIGHT: {
			TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(param); // Safetly cast generic param pointer to a specific type
			if (p != nullptr) this->player_right_input = p->Param;
			break;
		}
		case PLAYER_JUMP: {
			printf("jump!\n");
			TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(param); // Safetly cast generic param pointer to a specific type
			if (p != nullptr) this->player_jump_input = p->Param;
			break;
		}
		default:
			break;
		}
}
