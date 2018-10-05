#pragma once
//created by Game
#include <glm/glm.hpp>

class GameObject {
public:
	GameObject();
	glm::vec2 getPos();
	void setPos(glm::vec2 p);
private:
	glm::vec2 pos;
};