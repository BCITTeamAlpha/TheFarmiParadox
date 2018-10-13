#pragma once

#include <map>
#include <vector>
#include <windows.h>
#include "UIComponent.h"
#include "EventManager.h"

//has UIComponents; created by main

class UIManager {
public:
	UIManager(float width, float height);
    ~UIManager();

    void Resize();
    void AddToRoot(UIComponent *component);

    UIComponent *_root;
};