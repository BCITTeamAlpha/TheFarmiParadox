#pragma once

#include "Renderable.h"
#include "EventManager.h"
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
    UIComponent(float width, float height, float x, float y);
    ~UIComponent();

    virtual void Resize();
    virtual bool IsTransparent();

    void Add(UIComponent* child);

    bool                visible;
    glm::vec2           size;
    glm::vec2           anchor;
    VerticalAnchor      vAnchor;
    HorizontalAnchor    hAnchor;
    AnchorType          anchorType;
    UIComponent         *parent;

    glm::vec2           screenPosition;
    glm::vec2           screenSize;
    glm::vec3           rotation;

    std::vector<UIComponent*> children;
};