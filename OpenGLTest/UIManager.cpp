#include "UIManager.h"

std::map<std::string, FontType> UIManager::FontLibrary;

UIManager::UIManager(float width, float height) {
    // Create a transparent root element of the UI layout that covers the screen
    _root = new UIComponent(100, 100, 0, 0);
    _root->color = {0,0,0,0};
    _root->screenSize = {width, height};
    _root->screenPosition = {0, 0};

    _interactX = width / INTERACT_MAP_WIDTH;
    _interactY = height / INTERACT_MAP_HEIGHT;

    Resize();

    TypeParam<UIComponent*> param(_root);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);

    // Initialize our fonts (Maybe move this somewhere else because static?)
    initFont("ShareTechMono", "./ShareTechMono.png");
    
    EventManager::subscribe(UI_ADD_CLICK, this);
    EventManager::subscribe(UI_ADD_HOVER, this);
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

void UIManager::notify(EventName eventName, Param* params) {
    switch (eventName) {
    case UI_ADD_CLICK: {
        TypeParam<UIComponent*> *p = dynamic_cast<TypeParam<UIComponent*> *>(params);
        UIComponent* c = p->Param;
        
        float top, bottom, left, right,
              iTop, iBottom, iLeft, iRight;
        bottom = c->screenPosition.y;
        left = c->screenPosition.x;
        top = c->screenPosition.y + c->screenSize.y;
        right = c->screenPosition.x + c->screenSize.x;

        for (int x = 0; x < INTERACT_MAP_WIDTH; x++) {
            for (int y = 0; y < INTERACT_MAP_HEIGHT; y++) {
                iTop = y * _interactY + _interactY;
                iRight = x * _interactX + _interactX;
                iLeft = x * _interactX;
                iBottom = y * _interactY;
                if (top > iBottom && bottom < iTop &&
                    right > iLeft && left < iRight) {
                    _interactMap[x][y].push_back(c);
                }
            }
        }
        break;
    }
    case UI_CLICK: {
        UIComponent *top = nullptr;
        TypeParam<std::pair<int, int>> *p = dynamic_cast<TypeParam<std::pair<int, int>> *>(params);
        std::pair<int, int> coords = p->Param;
        for (UIComponent *c : _interactMap[coords.first / _interactX][coords.second / _interactY]) {
            if (pointInRect(coords.first, coords.second, c->screenPosition.y + c->screenSize.y, c->screenPosition.x + s->screenSize.x, c->screenPosition.x, c->screenPosition.y)) {
                if (top == nullptr || c->z > top->z) {
                    top = c;
                }
            }
        }
        if (top != nullptr) {
            top->ClickAction();
        }
        break;
    }
    default:
        break;
    }
}

UIComponent* UIManager::Root() { return _root; }

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

bool UIManager::pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom) {
    return (px > rLeft && px < rRight && py < rTop && px > rBottom);
}