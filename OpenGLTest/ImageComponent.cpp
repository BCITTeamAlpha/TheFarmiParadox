#include "ImageComponent.h"

ImageComponent::ImageComponent(std::string imagePath, float width, float height, float x, float y) :
    UIComponent(width, height, x, y), _imagePath(imagePath) {

	texture = AssetLoader::loadTexture(imagePath);
    aspectRatio = float(texture.width) / texture.height;
    color = {1,1,1,1};

    TypeParam<UIComponent*> param(this);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
}

bool ImageComponent::IsTransparent() { return true; }