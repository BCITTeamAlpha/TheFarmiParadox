#include "Renderable.h"

Renderable::Renderable() {
	z = 0.0f;
	scale = glm::vec3(1.0);
	color = glm::vec4(1.0);
	fullBright = false;
	roughness = 0.5f;
	f0 = 0.04;
}

glm::vec3 Renderable::getPosition3() {
	return glm::vec3(*position, z);
}