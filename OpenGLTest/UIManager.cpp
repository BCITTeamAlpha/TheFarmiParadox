#include "UIManager.h"

std::map<std::string, FontType> UIManager::FontLibrary;

UIManager::UIManager(float width, float height) {
    // Create a transparent root element of the UI layout that covers the screen
    _root = new UIComponent(100, 100, 0, 0);
    _root->_color = {0,0,0,0};
    _root->screenSize = {width, height};
    _root->screenPosition = {0, 0};

    Resize();

    TypeParam<UIComponent*> param(_root);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);

    // Initialize our fonts (Maybe move this somewhere else because static?)
    initFont("ShareTechMono", "./ShareTechMono.png");
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

void UIManager::initFont(std::string fontName, std::string path) {
    FontType newFont;
    newFont.Name = fontName;

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* texData = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    newFont.TextureData.assign(texData, texData + width * height * 4);
    newFont.TexHeight = height;
    newFont.TexWidth = width;

    UIManager::FontLibrary[fontName] = newFont;

    TypeParam<std::pair<std::string, std::string>> param(std::pair<std::string,std::string>(fontName, path));
    EventManager::notify(RENDERER_INIT_FONT, &param, false);
}