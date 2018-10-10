#pragma once
//created by Game
#include <glm/glm.hpp>
#include "Renderable.h"

class GameObject {
public:
	GameObject();
	glm::vec2 getPos();
	void setPos(glm::vec2 p);
	glm::vec3 getRot();
	void setRot(glm::vec3 p);
	void setRenderable(Renderable *r);
private:
	glm::vec2 position;
	glm::vec3 rotation;
	Renderable *renderable;
};