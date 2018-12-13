#include "Renderable.h"

Renderable::Renderable() {
	model = {};
	texture = {};
	invalidated = false;
	position = glm::vec3(0.0f);
	z = 0.0f;
	scale = glm::vec3(1.0f);
	color = glm::vec4(1.0f);
	fullBright = false;
	roughness = 0.5f;
	metallic = 0.0f;
	f0 = 0.04;
}

glm::vec3 Renderable::getPosition3() {
	return glm::vec3(position, z);
}