#include "PhysicsManager.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

const float PhysicsManager::VELOCITY_CAP = 40.0f;
const float player_speed = 10.0f;
const float player_jump_speed = 20.0f;

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
		glm::vec2 pos = object->position;
		glm::vec2 vel = object->velocity;
		glm::vec2 acc = gravAcceleration(pos);
		pos += vel * dTime;
		vel += acc * dTime;

		// clamp velocity magnitude
		if (length(vel) > VELOCITY_CAP) {
			vel = glm::normalize(vel) * VELOCITY_CAP;
		}

		// collision response
		glm::vec2 N;
		bool collided = object->colliding_with_map(*map, acc, N);
		glm::vec2 T = { N.y, -N.x };
		float N_comp = dot(N, vel);
		float T_comp = dot(T, vel);
		if (collided) {
			N_comp = std::max(0.0f, N_comp);
			T_comp = 0;
		}

		// player movement
		Character *character = dynamic_cast<Character *>(object);
		if (character) {
			float y_axis = player_jump_speed * character->jump_input;
			if (collided && y_axis != 0) {
				N_comp = y_axis;
			}
			float x_axis = player_speed * (character->right_input - character->left_input);
			if (x_axis != 0) {
				T_comp = x_axis;
			}
		}

		vel = N * N_comp + T * T_comp;

		object->position = pos;
		object->velocity = vel;
		object->rotation.z = atan2(acc.x, -acc.y) * 180.0f / M_PI;
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

//
//void PhysicsManager::notify(EventName eventName, Param* param) {
//	switch (eventName) {
//		case PLAYER_LEFT: {
//			TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(param); // Safetly cast generic param pointer to a specific type
//			if (p != nullptr) this->player_left_input = p->Param;
//			break;
//		}
//		case PLAYER_RIGHT: {
//			TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(param); // Safetly cast generic param pointer to a specific type
//			if (p != nullptr) this->player_right_input = p->Param;
//			break;
//		}
//		case PLAYER_JUMP: {
//			printf("jump!\n");
//			TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(param); // Safetly cast generic param pointer to a specific type
//			if (p != nullptr) this->player_jump_input = p->Param;
//			break;
//		}
//		default:
//			break;
//		}
//}
