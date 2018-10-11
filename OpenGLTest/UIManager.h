#pragma once
#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "UIComponent.h"
#include <map>
#include <vector>

//has UIComponents; created by main

class UIManager {
public:
	UIManager();

    void resize();
    void add(const UIComponent* component);
private:
    void sizeChildren(const UIComponent* parent);

    UIComponent                                         _root;
};

#endif