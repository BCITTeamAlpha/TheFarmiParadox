#pragma once
#include "PhysicsObject.h"

class Character : public PhysicsObject {
public:
	Character();
	bool controllable;
	bool left_input;
	bool right_input;
	bool jump_input;
};