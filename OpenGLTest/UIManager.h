#pragma once

#include <map>
#include <string>
#include <windows.h>
#include <tinyxml2/tinyxml2.h>
#include "AssetLoader.h"
#include "UIComponent.h"
#include "EventManager.h"

/**
Static structure used to store the UI data, and methods to interact with UI
*/
class UIManager : public ISubscriber {
public:
    UIManager(float width, float height);
    ~UIManager();

	// Resize the root element (and consequently every element)
    void Resize();

	// Add a new UIComponent to the root component
    static void AddToRoot(UIComponent *component);

    void notify(EventName eventName, Param* params);    // Overrides ISubscriber::notify

	// Get pointer to the root element
    static UIComponent* Root();

	// Search for a component by it's ID string, and return it
	static UIComponent* GetComponentById(std::string id);

	// Reads an XML file and populates the UI system with elements generated from it
    static void LoadFromXML(const char *filePath);

	// Define an on-click function passing in its label and the function to execute
    static void DefineClickFunction(const std::string name, void(*f)());
private:
	// Is the given point in the defined rectangle
    static bool pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom);

	// Recursive call for reading an XML element into a UIComponent
    static UIComponent* readChild(const tinyxml2::XMLElement* element);

	// Find the top element under your cursor that has an on-click event and store it in the top pointer location
	void findTopClick(UIComponent** top, UIComponent* comp, const float x, const float y);

	// Initial definition of all on-click functions
    void defineClicks();

    static UIComponent *_root;
    static std::map<const std::string, void(*)()> _clickFunctions;
};