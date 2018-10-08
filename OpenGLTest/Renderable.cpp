#include "Renderable.h"

GLuint Renderable::count = 0;

Renderable::Renderable() : _id(Renderable::count++) {}

glm::vec3 Renderable::getPosition3()
{
	std::cout << _position->x << ", " << _position->y << std::endl;
	return glm::vec3(*_position, _z);
}