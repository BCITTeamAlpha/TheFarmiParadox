#pragma once
#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "Renderable.h"
#include <vector>

//created by UIManager/AssetLoader?; used by Renderer

enum VerticalAnchor {
    ANCHOR_TOP, ANCHOR_VCENTER, ANCHOR_BOTTOM
};

enum HorizontalAnchor {
    ANCHOR_LEFT, ANCHOR_HCENTER, ANCHOR_RIGHT
};

enum AnchorType {
    ANCHOR_PERCENT, ANCHOR_PIXEL
};

class UIComponent : public Renderable {
public:
    UIComponent(float width, float height, float x, float y, float z = 0.0f);

    bool                visible;
    glm::vec2           size;
    glm::vec3           anchor;
    VerticalAnchor      vAnchor;
    HorizontalAnchor    hAnchor;
    AnchorType          anchorType;
    UIComponent         *parent;

    std::vector<UIComponent*> children;
private:

};

#endif