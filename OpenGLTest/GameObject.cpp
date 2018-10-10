#include "GameObject.h"

GameObject::GameObject()
{

}

void GameObject::setRenderable(Renderable *r)
{
	renderable = r;
	renderable->_position = &position;
	renderable->_rotation = &rotation;
}