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

void GameObject::set_position(glm::vec2 p) {
	position = p;
}

glm::vec2 GameObject::get_position() {
	return position;
}
