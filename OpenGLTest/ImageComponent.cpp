#include "ImageComponent.h"

ImageComponent::ImageComponent(std::string imagePath, float width, float height, float x, float y) :
    UIComponent(width, height, x, y), _imagePath(imagePath) {
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* texData = stbi_load(_imagePath.c_str(), &texture.width, &texture.height, &nrChannels, 0);
    texture.data.assign(texData, texData + texture.width * texture.height * 4);
    aspectRatio = float(texture.width) / texture.height;
    color = {1,1,1,1};

    TypeParam<UIComponent*> param(this);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
}

bool ImageComponent::IsTransparent() { return true; }