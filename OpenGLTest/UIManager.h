#pragma once

#include <map>
#include <string>
#include <windows.h>
#include <tinyxml2/tinyxml2.h>
#include "AssetLoader.h"
#include "UIComponent.h"
#include "EventManager.h"

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
    static void LoadFromXML(const char *filePath);
    static void DefineClickFunction(const std::string name, void(*f)());
private:
    static bool pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom);
    static UIComponent* readChild(const tinyxml2::XMLElement* element);

	void findTopClick(UIComponent** top, UIComponent* comp, const float x, const float y);

    static UIComponent *_root;
    static std::map<const std::string, void(*)()> _clickFunctions;
};