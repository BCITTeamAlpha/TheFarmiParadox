#include "Renderable.h"

GLuint Renderable::count = 0;

Renderable::Renderable() : _id(Renderable::count++) {}

glm::vec3 Renderable::getPosition3()
{
	return glm::vec3(*_position, _z);
}