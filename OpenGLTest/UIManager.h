#pragma once

#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include "UIComponent.h"
#include "EventManager.h"
#include "FontType.h"

//has UIComponents; created by main

class UIManager : public ISubscriber {
public:
    UIManager(float width, float height);
    ~UIManager();

    void Resize();
    void AddToRoot(UIComponent *component);

    void notify(EventName eventName, Param* params);    // Overrides ISubscriber::notify

    UIComponent *Root();

    static std::map<std::string, FontType> FontLibrary;
private:
    static void initFont(std::string fontName, std::string path);
    static bool pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom);

    const static int INTERACT_MAP_WIDTH = 16;
    const static int INTERACT_MAP_HEIGHT = 9;

    int _interactX;
    int _interactY;

    UIComponent                 *_root;
    std::vector<UIComponent*>   _interactMap[INTERACT_MAP_WIDTH][INTERACT_MAP_HEIGHT];
};