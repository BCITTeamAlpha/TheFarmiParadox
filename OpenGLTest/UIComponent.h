#pragma once

#include "Renderable.h"
#include "EventManager.h"
#include "stb_image.h"
#include <string>
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

enum UnitType {
    UNIT_PIXEL, UNIT_PERCENT, UNIT_SCALE
};

class UIComponent : public Renderable {
public:
    UIComponent(float width, float height, float x, float y);
    ~UIComponent();

    virtual void Resize();
    virtual bool IsTransparent();

    void Add(UIComponent* child);

	std::string			id;
    bool                visible;
    glm::vec2           size;
    glm::vec2           anchor;
    VerticalAnchor      vAnchor;
    HorizontalAnchor    hAnchor;
    AnchorType          anchorXType;
    AnchorType          anchorYType;
    UnitType            xType;
    UnitType            yType;
    float               aspectRatio;
    UIComponent         *parent;

    glm::vec2           screenPosition;
    glm::vec2           screenSize;
    glm::vec3           screenRotation;

    std::string         ClickAction;

    std::vector<UIComponent*> children;
};