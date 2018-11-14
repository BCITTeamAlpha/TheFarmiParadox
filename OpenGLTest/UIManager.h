#pragma once

#include <map>
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
    static void AddToRoot(UIComponent *component);

    void notify(EventName eventName, Param* params);    // Overrides ISubscriber::notify

    static UIComponent* Root();
	static UIComponent* GetComponentById(std::string id);

    static std::map<std::string, FontType> FontLibrary;
private:
    static void initFont(std::string fontName, std::string path);
    static bool pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom);

	void findTopClick(UIComponent** top, UIComponent* comp, const float x, const float y);

    static UIComponent *_root;
};