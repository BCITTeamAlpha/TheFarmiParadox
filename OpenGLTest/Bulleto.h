#pragma once
#include "PhysicsObject.h"

class Bulleto : public PhysicsObject {
public:
	Bulleto() {

	}

	bool colliding_with_player(glm::vec2 & player_pos) {
		float length = glm::length(get_position() - player_pos);

		return length < (this->radius + 1.0f);
	}

private:

};