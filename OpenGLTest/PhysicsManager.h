#pragma once
//Instantiated in the game class, uses the event system
#include "Map.h"
#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include <iostream>
#include "EventManager.h"

class PhysicsManager : public ISubscriber {
public:
	PhysicsManager(std::vector<Planetoid> *p, Map *m);
	void PhysicsManager::calcPhysics(float dTime);
	void addObject(PhysicsObject *obj);
	static const float VELOCITY_CAP;

	void ISubscriber::notify(EventName eventName, Param* param);

	
private:
	void characterMovement(PhysicsObject *object);
	glm::vec2 gravAcceleration(glm::vec2 pos);
	std::vector<Planetoid> *planets;
	std::vector<PhysicsObject*> objects;
	Map *map;
	float input_X;
	float player_jump_input;
};