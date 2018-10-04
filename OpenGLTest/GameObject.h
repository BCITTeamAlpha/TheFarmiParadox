#pragma once
//created by Game
#include <glm/glm.hpp>

class GameObject {
public:
	GameObject();
	glm_f64vec2 getPos();
private:
	glm_f64vec2 pos;
};