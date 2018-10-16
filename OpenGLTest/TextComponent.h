#pragma once

#include "UIManager.h"

class TextComponent : public UIComponent {
public:
    static const std::string DEFAULT_FONT;

    TextComponent(std::string text, float fontSize, float x, float y);

    void Resize();
    bool IsTransparent();

    void SetText(std::string text);
private:
    glm::vec2 getUVfromChar(const char c);
    void generateVertices();

    std::string _text;
    float       _fontSize;
};