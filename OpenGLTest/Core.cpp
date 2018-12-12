#include "Core.h"

#include "AssetLoader.h"
#include "Renderable.h"

Core::Core(Planetoid p) {
	set_position(p._pos);
	radius = p._r * 0.5f;

	renderable = std::make_shared<Renderable>(Renderable());
	renderable->position = get_position();
	renderable->z = 0;
	renderable->rotation = glm::vec3(90, 0, 0);
	renderable->scale = glm::vec3(radius, 3.0f, radius);
	renderable->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	renderable->fullBright = true;
	renderable->texture = AssetLoader::loadTexture("./magma.png");
	renderable->model = AssetLoader::loadModel("../Models/Cylinder.obj");
}
