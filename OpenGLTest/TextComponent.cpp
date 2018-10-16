#include "TextComponent.h"

const std::string TextComponent::DEFAULT_FONT = "ShareTechMono";

TextComponent::TextComponent(std::string text, float fontSize, float x, float y, std::string fontType) :
    UIComponent(0, 0, x, y), _text(text), _fontSize(fontSize) {
    _font = UIManager::FontLibrary[fontType];
}

void TextComponent::SetText(std::string text) {
    _text = text;
    Resize();
}

void TextComponent::Resize() {
    if (parent != nullptr) {
        _textureLocation = _font.TextureLocation;

        float fontWidth = _fontSize * _font.AspectRatio;

        screenSize.y = _fontSize;
        screenSize.x = fontWidth * _text.length();

        glm::vec2 screenAnchor;
        screenAnchor.x = anchorXType == ANCHOR_PERCENT ? anchor.x / 100.0f * parent->screenSize.x / 2.0f : anchor.x;
        screenAnchor.y = anchorYType == ANCHOR_PERCENT ? anchor.y / 100.0f * parent->screenSize.y / 2.0f : anchor.y;

        switch (hAnchor) {
        case ANCHOR_LEFT:
            screenPosition.x = parent->screenPosition.x + screenAnchor.x;
            break;
        case ANCHOR_HCENTER:
            screenPosition.x = parent->screenPosition.x + parent->screenSize.x / 4 - screenSize.x / 4 + screenAnchor.x;
            break;
        case ANCHOR_RIGHT:
            screenPosition.x = parent->screenPosition.x + parent->screenSize.x / 2 - screenSize.x / 2 - screenAnchor.x;
            break;
        }

        switch (vAnchor) {
        case ANCHOR_TOP:
            screenPosition.y = parent->screenPosition.y + parent->screenSize.y / 2 - screenSize.y / 2 - screenAnchor.y;
            break;
        case ANCHOR_VCENTER:
            screenPosition.y = parent->screenPosition.y + parent->screenSize.y / 4 - screenSize.y / 4 + screenAnchor.y;
            break;
        case ANCHOR_BOTTOM:
            screenPosition.y = parent->screenPosition.y + screenAnchor.y;
            break;
        }

        _z = parent->_z + 1;

        generateVertices();

        TypeParam<UIComponent*> param(this);
        EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
    }
}

bool TextComponent::IsTransparent() {
    return true;
}

glm::vec2 TextComponent::getUVfromChar(const char c) {
    int ascii = int(c);

    int index = 0;
    if (ascii >= 32 && ascii <= 126) {
        index = ascii - 32;
    } else if (ascii == 8216 || ascii == 8217) {
        index = ascii - 8121;
    } else if (ascii == 8220 || ascii == 8221) {
        index = ascii - 8123;
    }

    glm::vec2 uv;
    uv.x = 0.1f * (index % 10);
    uv.y = 1.0f - (0.1f * (index / 10));

    return uv;
}

void TextComponent::generateVertices() {
    glm::vec2 uv;
    float fontWidth = _fontSize * _font.AspectRatio;

    _positions.clear();
    _texCoords.clear();
    _elements.clear();

    for (int i = 0; i < _text.length(); i++) {
        uv = getUVfromChar(_text[i]);
        _positions.push_back(glm::vec3(screenPosition.x + fontWidth * i, screenPosition.y + _fontSize, 0)); // Top Left
        _positions.push_back(glm::vec3(screenPosition.x + fontWidth * i + fontWidth, screenPosition.y + _fontSize, 0)); // Top Right
        _positions.push_back(glm::vec3(screenPosition.x + fontWidth * i, screenPosition.y, 0)); // Bottom Left
        _positions.push_back(glm::vec3(screenPosition.x + fontWidth * i + fontWidth, screenPosition.y, 0));
        _texCoords.push_back(uv);
        _texCoords.push_back(uv + glm::vec2(0.1, 0));
        _texCoords.push_back(uv + glm::vec2(0, -0.1));
        _texCoords.push_back(uv + glm::vec2(0.1, -0.1));
        _elements.push_back(i * 4 + 1);
        _elements.push_back(i * 4 + 0);
        _elements.push_back(i * 4 + 2);
        _elements.push_back(i * 4 + 1);
        _elements.push_back(i * 4 + 2);
        _elements.push_back(i * 4 + 3);
    }
}