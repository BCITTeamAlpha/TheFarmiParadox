#include "GameObject.h"

GameObject::GameObject()
{

}

glm::vec2 GameObject::getPos()
{
	return position;
}

void GameObject::setPos(glm::vec2 p)
{
	position = p;
}

glm::vec3 GameObject::getRot()
{
	return rotation;
}

void GameObject::setRot(glm::vec3 p)
{
	rotation = p;
}

void GameObject::setRenderable(Renderable *r)
{
	renderable = r;
	renderable->_position = &position;
	renderable->_rotation = &rotation;
}