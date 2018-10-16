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
    _positions = { {-2.5,2.5,0},{2.5,2.5,0}, {-2.5,-2.5,0}, {2.5,-2.5,0} };
    _color = {0,0,0,0};

    TypeParam<UIComponent*> param(this);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
}

UIComponent::~UIComponent() {
    for (UIComponent *child : children)
        delete child;
}

void UIComponent::Resize() {
    if (parent != nullptr) {
        screenSize = parent->screenSize * (size / 100.0f);
        glm::vec2 screenAnchor = anchorType == ANCHOR_PERCENT ? anchor / 100.0f * parent->screenSize / 2.0f : anchor;

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
    }
    
    _positions = {
        { screenPosition.x, screenPosition.y + screenSize.y, 0 }, // Top Left
        { screenPosition + screenSize, 0 }, // Top Right
        { screenPosition, 0 }, // Bottom Left
        { screenPosition.x + screenSize.x, screenPosition.y, 0 }  // Bottom Right
    };

    TypeParam<UIComponent*> param(this);
    EventManager::notify(RENDERER_POPULATE_BUFFERS, &param, false);

    for (UIComponent *child : children) {
        child->Resize();
    }
}

void UIComponent::Add(UIComponent* child) {
    child->parent = this;
    children.push_back(child);
    child->Resize();
}

bool UIComponent::IsTransparent() {
    return _color.a < 1.0f;
}

