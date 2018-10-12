#pragma once

#include "UIComponent.h"
#include <map>
#include <vector>

//has UIComponents; created by main

class UIManager {
public:
	UIManager(float width, float height);

    void Resize(float width, float height);
    void AddToRoot(UIComponent *component);
private:
    UIComponent _root;
};