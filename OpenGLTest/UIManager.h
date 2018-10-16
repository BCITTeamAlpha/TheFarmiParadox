#pragma once

#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include "UIComponent.h"
#include "EventManager.h"
#include "FontType.h"

//has UIComponents; created by main

class UIManager {
public:
	UIManager(float width, float height);
    ~UIManager();

    void Resize();
    void AddToRoot(UIComponent *component);

    UIComponent *_root;
    
    static std::map<std::string, FontType> FontLibrary;
private:
    static void initFont(std::string fontName, std::string path, float aspectRatio);
};