#include "UIComponent.h"

UIComponent::UIComponent(float width, float height, float x, float y, float z) :
    size(width, height), anchor(x, y, z) {

    visible = true;
    vAnchor = ANCHOR_TOP;
    hAnchor = ANCHOR_LEFT;
    anchorType = ANCHOR_PIXEL;
    parent = nullptr;
}

