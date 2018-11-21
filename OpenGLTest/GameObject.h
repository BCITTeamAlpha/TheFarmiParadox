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

	void set_position(glm::vec2 p);
	glm::vec2 get_position();
	void set_rotation(glm::vec3 r);
	glm::vec3 get_rotation();

	std::shared_ptr<Renderable> renderable;
private:
	glm::vec2 position;
	glm::vec3 rotation;
};