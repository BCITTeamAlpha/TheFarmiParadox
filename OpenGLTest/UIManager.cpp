#include "UIManager.h"

UIManager::UIManager(float width, float height) : 
    _root(100, 100, 0, 0) {
    _root._color = {1,0,0,0.1};
    _root.screenSize = {width/2, height/2};
    _root.screenPosition = {0, 0};

    Resize();

    TypeParam<UIComponent*> param(&_root);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
}

void UIManager::Resize() {
    _root.Resize();
}

void UIManager::AddToRoot(UIComponent *component) {
    _root.Add(component);
}