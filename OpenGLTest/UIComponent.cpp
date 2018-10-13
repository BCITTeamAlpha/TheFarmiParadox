#include "UIComponent.h"

UIComponent::UIComponent(float width, float height, float x, float y) :
    size(width, height), anchor(x, y) {

    visible = true;
    vAnchor = ANCHOR_TOP;
    hAnchor = ANCHOR_LEFT;
    anchorType = ANCHOR_PIXEL;
    parent = nullptr;

    _position = &screenPosition;
    _rotation = &rotation;
    _z = 0;

    _normals = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
    _elements = { 1, 0, 2, 1, 2, 3 };
    _texCoords = { { 0, 1 },{ 1, 1 },{ 0, 0 },{ 1, 0 } };
    _color = {0,0,0,0};
}

UIComponent::~UIComponent() {
    for (UIComponent *child : children)
        delete child;
}

void UIComponent::Resize() {
    if (parent != nullptr) {
        screenSize = parent->screenSize * (size / 100.0f);

        switch (hAnchor) {
        case ANCHOR_LEFT:
            screenPosition.x = parent->screenPosition.x + anchor.x;
            break;
        case ANCHOR_HCENTER:
            screenPosition.x = parent->screenPosition.x + parent->screenSize.x / 2 - screenSize.x / 2 + anchor.x;
            break;
        case ANCHOR_RIGHT:
            screenPosition.x = parent->screenPosition.x + parent->screenSize.x - screenSize.x - anchor.x;
            break;
        }

        switch (vAnchor) {
        case ANCHOR_TOP:
            screenPosition.y = parent->screenPosition.y + parent->screenSize.y - screenSize.y - anchor.y;
            break;
        case ANCHOR_VCENTER:
            screenPosition.y = parent->screenPosition.y + parent->screenSize.y / 2 - screenSize.y / 2 + anchor.y;
            break;
        case ANCHOR_BOTTOM:
            screenPosition.y = parent->screenPosition.y + anchor.y;
            break;
        }

        _z = parent->_z + 1;
    }
    
    _positions = {
        { screenPosition.x, screenPosition.y + screenSize.y, 0 }, // Top Left
        { screenPosition + screenSize, 0 }, // Top Right
        { screenPosition, 0 }, // Bottom Left
        { screenPosition.x + screenSize.x, screenPosition.y, 0 }  // Bottom Right
    };

    for (UIComponent *child : children) {
        child->Resize();
    }
}

void UIComponent::Add(UIComponent* child) {
    child->parent = this;
    children.push_back(child);

    TypeParam<UIComponent*> param(child);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
}

