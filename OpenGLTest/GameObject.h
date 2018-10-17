#pragma once
//created by Game
#include <glm/glm.hpp>
#include "Renderable.h"

class GameObject {
public:
	GameObject();
	void setRenderable(Renderable *r);
	glm::vec2 position;
	glm::vec3 rotation;
private:
	Renderable *renderable;
};