#include "GameObject.h"

GameObject::GameObject()
{

}

glm::vec2 GameObject::getPos()
{
	return pos;
}

void GameObject::setPos(glm::vec2 p)
{
	pos = p;
}