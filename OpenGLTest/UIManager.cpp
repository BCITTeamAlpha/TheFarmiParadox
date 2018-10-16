#include "UIManager.h"

std::map<std::string, GLuint> UIManager::FontTextures;

UIManager::UIManager(float width, float height) {
    _root = new UIComponent(100, 100, 0, 0);
    _root->_color = {0,0,0,0};
    _root->screenSize = {width, height};
    _root->screenPosition = {0, 0};

    Resize();

    TypeParam<UIComponent*> param(_root);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);

    initFont("ShareTechMono");
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

void UIManager::initFont(std::string fontName) {
    TypeParam<std::string> param(fontName);
    EventManager::notify(RENDERER_INIT_FONT, &param, false);
}