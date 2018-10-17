#include "ImageComponent.h"

ImageComponent::ImageComponent(std::string imagePath, float width, float height, float x, float y) :
    UIComponent(width, height, x, y), _imagePath(imagePath) {
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* texData = stbi_load(_imagePath.c_str(), &_texWidth, &_texHeight, &nrChannels, 0);
    _texture.assign(texData, texData + _texWidth * _texHeight * 4);
    aspectRatio = float(_texWidth) / _texHeight;
    _color = {1,1,1,1};

    TypeParam<UIComponent*> param(this);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
}

bool ImageComponent::IsTransparent() { return true; }