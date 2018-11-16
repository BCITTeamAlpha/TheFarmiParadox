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
		glm::vec2 max_acc;
		glm::vec2 acc = gravAcceleration(pos, max_acc);
		pos += vel * dTime;
		vel += acc * dTime;

		// player movement
		bool player_input = false;
		Character *character = dynamic_cast<Character *>(object);
		if (character && character->controllable && character->grounded) {
			glm::vec2 N_acc = -glm::normalize(max_acc);
			glm::vec2 T_acc = { N_acc.y, -N_acc.x };

			float N_comp = dot(N_acc, vel);
			float T_comp = dot(T_acc, vel);

			if (character->jump_input) {
				player_input = true;
				N_comp = player_jump_speed;
			}
		   	if (character->left_input || character->right_input) {
				player_input = true;
				T_comp = player_speed * (character->right_input - character->left_input);
			}

			vel = N_acc * N_comp + T_acc * T_comp;
		}

		// collision response
		glm::vec2 N_col;
		size_t colliding = object->colliding_with_map(*map, pos, max_acc, N_col);
		glm::vec2 T_col = { N_col.y, -N_col.x };
		if (colliding == 4) {
			// if glitching into ground, don't
			pos = object->position;
			vel = glm::vec2(0);
		} else if (colliding) {
			float N_comp = dot(N_col, vel);
			float T_comp = dot(T_col, vel);

			if (N_comp < 0) {
				N_comp = 0;
			}
			if (!player_input) {
				T_comp *= 0.5;
			}

			vel = N_col * N_comp + T_col * T_comp;
		}

		// clamp velocity magnitude
		if (length(vel) > VELOCITY_CAP) {
			vel = glm::normalize(vel) * VELOCITY_CAP;
		}

		object->position = pos;
		object->velocity = vel;
		object->grounded = colliding;
		object->rotation.z = atan2(acc.x, -acc.y) * 180.0f / M_PI;
	}
}

//Finds the net force on a given point in space
glm::vec2 PhysicsManager::gravAcceleration(glm::vec2 pos, glm::vec2 &max_acceleration)
{
	glm::vec2 net_acceleration = glm::vec2(0, 0);
	float max_acceleration_magnitude = 0;

	//calculate the force of each planetoid and add them to the vector
	for (Planetoid p : *planets)
	{
		//calculate distance between planet and object
		glm::vec2 direction = p._pos - pos;
		float distance = glm::length(direction);

		//calculate the force from each planetoid
		float magnitude = p._m / (distance * distance);
		glm::vec2 acceleration = glm::normalize(direction) * magnitude;

		//add force to the total force
		if (magnitude > max_acceleration_magnitude) {
			max_acceleration = acceleration;
			max_acceleration_magnitude = magnitude;
		}

		net_acceleration += acceleration;
	}

	return net_acceleration;
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
