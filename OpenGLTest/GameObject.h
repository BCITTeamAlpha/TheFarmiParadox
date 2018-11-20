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
	glm::vec3 rotation;
	std::shared_ptr<Renderable> renderable;

	void set_position(glm::vec2 p);
	glm::vec2 get_position();
private:
	glm::vec2 position;
};