#pragma once
//Instantiated in the game class, uses the event system
#include "Map.h"
#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include <iostream>
#include "EventManager.h"
#include "Character.h"

class PhysicsManager/* : public ISubscriber*/ {
public:
	PhysicsManager(std::vector<Planetoid> *p, Map *m);
	void calcPhysics(float dTime);
	void addObject(PhysicsObject *obj);
	static const float VELOCITY_CAP;

	//void ISubscriber::notify(EventName eventName, Param* param);
	static void hello()
	{

	}

	
private:
	void characterMovement(Character *Character);
	glm::vec2 gravAcceleration(glm::vec2 pos);
	std::vector<Planetoid> *planets;
	std::vector<PhysicsObject*> objects;
	std::vector<Character*> chars;
	Map *map;
	/*bool player_left_input;
	bool player_right_input;
	bool player_jump_input;*/
};