#include "UIComponent.h"

UIComponent::UIComponent(UIComponent *parentComponent, float width, float height, float x, float y, float z = 0.0f) :
    size(width, height), anchor(x, y) {

    visible = true;
    vAnchor = ANCHOR_TOP;
    hAnchor = ANCHOR_LEFT;
    anchorType = ANCHOR_PIXEL;
    parent = parentComponent;

    Renderable::_position = &screenPosition;
    Renderable::_rotation = &rotation;
    Renderable::_z = z;
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
            screenPosition.y = parent->screenPosition.y + anchor.y;
            break;
        case ANCHOR_VCENTER:
            screenPosition.y = parent->screenPosition.y + parent->screenSize.y / 2 - screenSize.y / 2 + anchor.y;
            break;
        case ANCHOR_BOTTOM:
            screenPosition.y = parent->screenPosition.y + parent->screenSize.y - screenSize.y - anchor.y;
            break;
        }
    }

    for (UIComponent *child : children)
        child->Resize();
}
