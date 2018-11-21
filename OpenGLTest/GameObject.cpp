#include "GameObject.h"

GameObject::GameObject() {

}

GameObject::~GameObject() {

}

void GameObject::setRenderable(Renderable *r) {
	renderable.reset(r);
	renderable->position = position;
	renderable->rotation = rotation;
}

void GameObject::set_position(glm::vec2 p) {
	position = p;
	if (renderable != NULL) {
		renderable->position = position;
	}
}

glm::vec2 GameObject::get_position() {
	return position;
}

void GameObject::set_rotation(glm::vec3 r) {
	rotation = r;
	if (renderable != NULL) {
		renderable->rotation = rotation;
	}
}

glm::vec3 GameObject::get_rotation() {
	return rotation;
}