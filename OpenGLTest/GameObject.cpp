#include "GameObject.h"

GameObject::GameObject()
{

}
GameObject::~GameObject()
{

}

void GameObject::setRenderable(Renderable *r)
{
	renderable = r;
	renderable->position = &position;
	renderable->rotation = &rotation;
}