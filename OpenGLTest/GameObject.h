#pragma once
//created by Game
#include <memory>
#include <glm/glm.hpp>
#include "Renderable.h"

class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	void setRenderable(Renderable *r);
	glm::vec2 position;
	glm::vec3 rotation;
	std::shared_ptr<Renderable> renderable;
};