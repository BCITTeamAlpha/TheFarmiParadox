#include "UIManager.h"
#include <stack>

std::map<std::string, FontType> UIManager::FontLibrary;
UIComponent* UIManager::_root;


UIManager::UIManager(float width, float height) {
    // Create a transparent root element of the UI layout that covers the screen
    _root = new UIComponent(100, 100, 0, 0);
    _root->color = {0,0,0,0};
    _root->screenSize = {width, height};
    _root->screenPosition = {0, 0};

    Resize();

    TypeParam<UIComponent*> param(_root);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);

    // Initialize our fonts (Maybe move this somewhere else because static?)
    initFont("ShareTechMono", "./ShareTechMono.png");
    
    EventManager::subscribe(UI_CLICK, this);
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
    case UI_CLICK: {
        TypeParam<std::pair<float, float>> *p = dynamic_cast<TypeParam<std::pair<float, float>> *>(params);
        std::pair<float, float> coords = p->Param;
		UIComponent* select = _root;
		findTopClick(&select, _root, coords.first, _root->screenSize.y - coords.second);
        if (select != _root) {
			select->ClickAction();
        }
        break;
    }
    default:
        break;
    }
}

UIComponent* UIManager::Root() { return _root; }

UIComponent* UIManager::GetComponentById(std::string id) {
	UIComponent* component = nullptr;

	std::stack<UIComponent*> stack;
	stack.push(_root);
	while (!stack.empty()) {
		UIComponent* node = stack.top();
		stack.pop();

		if (node->id == id) {
			component = node;
			break;
		}

		for (UIComponent* child : node->children)
			stack.push(child);
	}

	return component;
}

void UIManager::findTopClick(UIComponent** top, UIComponent* comp, const float x, const float y) {
	for (UIComponent* c : comp->children) {
		if (c->visible && pointInRect(x/2, y, c->screenPosition.y + c->screenSize.y,
			c->screenPosition.x + c->screenSize.x, c->screenPosition.x, c->screenPosition.y)) {
			if (c->ClickAction != nullptr && c->z > (*top)->z) {
				*top = c;
			}
			findTopClick(top, c, x, y);
		}
	}
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

bool UIManager::pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom) {
    return (px > rLeft && px < rRight && py < rTop && px > rBottom);
}