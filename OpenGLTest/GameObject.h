#pragma once
//created by Game
#include <glm/glm.hpp>
#include "Renderable.h"

class GameObject {
public:
	GameObject();
	glm::vec3 getPos();
	void setPos(glm::vec3 p);
	glm::vec3 getRot();
	void setRot(glm::vec3 p);
	void setRenderable(Renderable *r);
private:
	glm::vec3 position;
	glm::vec3 rotation;
	Renderable *renderable;
};