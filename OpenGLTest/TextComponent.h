#pragma once

#include "UIComponent.h"

class TextComponent : public UIComponent {
public:
    TextComponent(std::string text, float width, float height, float x, float y);

    void SetText(std::string text);
private:
    std::string _text;
};