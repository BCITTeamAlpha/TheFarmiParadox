#include "GameObject.h"

GameObject::GameObject()
{

}
GameObject::~GameObject()
{

}

void GameObject::setRenderable(Renderable *r)
{
	renderable.reset(r);
	renderable->position = &position;
	renderable->rotation = &rotation;
}