#pragma once

#include "UIComponent.h"
#include "EventManager.h"

class ImageComponent : public UIComponent {
public:
    ImageComponent(std::string imagePath, float width, float height, float x, float y);

    bool IsTransparent();
private:
    std::string _imagePath;
};