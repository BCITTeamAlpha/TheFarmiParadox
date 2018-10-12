#include "UIManager.h"

UIManager::UIManager(float width, float height) : 
    _root(nullptr, 100, 100, 0, 0) {
    _root.screenPosition = glm::vec2(0, 0);
    _root.screenSize = glm::vec2(width, height);
}

void UIManager::Resize(float width, float height) {
    _root.screenSize = glm::vec2(width, height);
    _root.Resize();
}

void UIManager::AddToRoot(UIComponent *component) {
    component->parent = &_root;
    _root.children.push_back(component);
}