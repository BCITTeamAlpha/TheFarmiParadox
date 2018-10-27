#include "UIManager.h"

UIManager::UIManager(float width, float height) {
    // Create a transparent root element of the UI layout that covers the screen
    _root = new UIComponent(100, 100, 0, 0);
    _root->color = {0,0,0,0};
    _root->screenSize = {width, height};
    _root->screenPosition = {0, 0};

    Resize();

    TypeParam<UIComponent*> param(_root);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
}

UIManager::~UIManager() {
    delete _root;
}

void UIManager::Resize() {
    _root->Resize();
}

void UIManager::AddToRoot(UIComponent *component) {
    _root->Add(component);
}