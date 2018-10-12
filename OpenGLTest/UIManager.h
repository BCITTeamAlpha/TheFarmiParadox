#pragma once
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "UIComponent.h"
#include <map>
#include <vector>

//has UIComponents; created by main

class UIManager {
public:
	UIManager(float width, float height);

    void resize();
    void add(const UIComponent* component);
private:
    UIComponent _root;
    glm::vec2   _screenSize;
};

#endif