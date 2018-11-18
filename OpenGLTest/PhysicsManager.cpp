#include "PhysicsManager.h"

const float PhysicsManager::VELOCITY_CAP = 40.0f;
const float player_speed = 10.0f;
const float player_jump_speed = 20.0f;
const float FRAME_ROT = 500.0f;

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

		float desiredRot = atan2(max_acc.x, -max_acc.y) * 180.0f / M_PI;

		if (std::abs(desiredRot - object->rotation.z) < FRAME_ROT * dTime)
		{
			object->rotation.z = desiredRot;
		}
		else
		{
			float normRot = normalizeAngle(object->rotation.z);
			float normDRot = normalizeAngle(desiredRot);

			if (std::abs(normRot - normDRot) > 180)
			{
				if (normRot > normDRot)
					object->rotation.z += FRAME_ROT * dTime;
				else
					object->rotation.z -= FRAME_ROT * dTime;
			}
			else
			{
				if (normRot > normDRot)
					object->rotation.z -= FRAME_ROT * dTime;
				else
					object->rotation.z += FRAME_ROT * dTime;
			}
		}

		object->position = pos;
		object->velocity = vel;
		object->grounded = colliding;
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

float PhysicsManager::normalizeAngle(float angle)
{
	if (angle >= 0.0f && angle < 360.0f)
		return angle;

	if (angle < 0.0f)
		angle = 360.0f + std::fmod(angle, 360.0f);
	else
		angle = std::fmod(angle, 360.0f);

	return angle;
}


void PhysicsManager::addObject(PhysicsObject *obj)
{
	objects.push_back(obj);
}

glm::vec2 PhysicsManager::genSpawnPos()
{
	int *pRands = (int*)malloc(sizeof(int) * planets->size());
	int sum = 0;

	for (int i = 0; i < planets->size(); i++)
	{
		sum += planets->at(i)._r;
		pRands[i] = sum;
	}

	int rand = std::rand() % sum;
	
	for (int i = 0; i < planets->size(); i++)
	{
		if (rand < pRands[i])
			return posOnPlanet(i);
	}
}

glm::vec2 PhysicsManager::posOnPlanet(int pInd)
{
	int xStart = planets->at(pInd)._r + 1;
	int yStart = 0;

	int x = -1, y = -1;

	while (x < 0 || y < 0 || x > 128 || y > 128)
	{
		float rand = std::rand() % 360;
		rand *= 3.14f / 180.0f;

		std::cout << "Planet = " << planets->at(pInd)._x << ", " << planets->at(pInd)._y << " r = " << planets->at(pInd)._r << std::endl;
		std::cout << "Rand = " << rand << std::endl;

		x = xStart * std::cos(rand) - yStart * std::sin(rand) + planets->at(pInd)._x;
		y = yStart * std::cos(rand) + xStart * std::sin(rand) + planets->at(pInd)._y;

		std::cout << "X = " << x << std::endl;
		std::cout << "Y = " << y << std::endl;
	}

	return glm::vec2(x, y);
}