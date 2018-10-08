#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{

}

float PhysicsObject::getMass()
{
	return mass;
}

glm::vec2 PhysicsObject::getVelocity()
{
	return velocity;
}

void PhysicsObject::setVelocity(glm::vec2 v)
{
	velocity = v;
}