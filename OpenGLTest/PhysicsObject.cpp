#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{

}

float PhysicsObject::getMass()
{
	return mass;
}

glm::vec3 PhysicsObject::getVelocity()
{
	return velocity;
}

void PhysicsObject::setVelocity(glm::vec3 v)
{
	velocity = v;
}