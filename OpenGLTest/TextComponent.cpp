#include "TextComponent.h"

TextComponent::TextComponent(std::string text, float width, float height, float x, float y) :
    UIComponent(width, height, x, y) {
    SetText(text);
}

void TextComponent::SetText(std::string text) {

}