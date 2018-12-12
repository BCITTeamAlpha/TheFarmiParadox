#include "UIManager.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include <stack>
#include <sstream>

using namespace tinyxml2;

UIComponent* UIManager::_root;
std::map<const std::string, void(*)()> UIManager::_clickFunctions;

UIManager::UIManager(float width, float height) {
    UIComponent *blackOverlay = new UIComponent(100, 100, 0, 0);
    blackOverlay->id = "BlackOverlay";
    blackOverlay->color = { 0, 0, 0, 0 };

    // Create a transparent root element of the UI layout that covers the screen
    _root = new UIComponent(100, 100, 0, 0);
    _root->color = {0,0,0,0};
    _root->screenSize = {width, height};
    _root->screenPosition = {0, 0};
    _root->children.push_back(blackOverlay);
    blackOverlay->parent = _root;

    LoadFromXML("../UI/DefaultUI.xml");

    Resize();

    TypeParam<UIComponent*> param(_root);
    EventManager::notify(RENDERER_ADD_TO_UIRENDERABLES, &param, false);
    EventManager::subscribe(UI_CLICK, this);

    defineClicks();
}

UIManager::~UIManager() {
    delete _root;
}

void UIManager::Resize() {
    _root->Resize();

    GetComponentById("BlackOverlay")->z = 100;
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
			_clickFunctions[select->ClickAction]();
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

    if (_root != nullptr) {
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
    }

	return component;
}

void UIManager::LoadFromXML(const char *filePath) {
    tinyxml2::XMLDocument doc;
    XMLError err = doc.LoadFile(filePath);
    if (err == XML_SUCCESS) {
        XMLElement *root = doc.RootElement();

        XMLElement *child = root->FirstChildElement();
        while (child != nullptr) {
            UIComponent *childComp = readChild(child);
            childComp->parent = _root;
            _root->children.push_back(childComp);
            child = child->NextSiblingElement();
        }
    }
}

void UIManager::DefineClickFunction(const std::string name, void(*f)()) {
    _clickFunctions[name] = f;
}

void UIManager::findTopClick(UIComponent** top, UIComponent* comp, const float x, const float y) {
	for (UIComponent* c : comp->children) {
		if (c->visible && pointInRect(x, y, c->screenPosition.y + c->screenSize.y,
			c->screenPosition.x + c->screenSize.x, c->screenPosition.x, c->screenPosition.y)) {
			if (c->ClickAction != "" && c->z > (*top)->z) {
				*top = c;
			}
			findTopClick(top, c, x, y);
		}
	}
}

bool UIManager::pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom) {
    return (px > rLeft && px < rRight && py < rTop && px > rBottom);
}

void UIManager::defineClicks() {
    DefineClickFunction("toggleKaren", []() {
        UIComponent *karen = UIManager::GetComponentById("karen");
        if (karen->anchor.y < 0)
            karen->anchor.y = 0;
        else
            karen->anchor.y = -47;
        karen->Resize();
    });

    DefineClickFunction("startGame", []() {
        int numPlayer = 2;
        int *models = new int[4];
        UIComponent *comp = UIManager::GetComponentById("dataPNum");
        if (comp != nullptr)
            numPlayer = comp->size.x;

        comp = UIManager::GetComponentById("dataP1Model");
        if (comp != nullptr)
            models[0] = comp->size.x;

        comp = UIManager::GetComponentById("dataP2Model");
        if (comp != nullptr)
            models[1] = comp->size.x;

        comp = UIManager::GetComponentById("dataP3Model");
        if (comp != nullptr)
            models[2] = comp->size.x;

        comp = UIManager::GetComponentById("dataP4Model");
        if (comp != nullptr)
            models[3] = comp->size.x;

        TypeParam<std::pair<int,int*>> *param = new TypeParam<std::pair<int,int*>>(std::pair<int,int*>(numPlayer, models));
        EventManager::notify(GAME_START, param);
    });

    DefineClickFunction("playButton", []() {
        UIComponent *comp = UIManager::GetComponentById("modelSelect");
        comp->visible = true;

        EventManager::notify(MENU_SHOW_MODEL, nullptr);

        comp = UIManager::GetComponentById("splashScreen");
        comp->visible = false;
    });

    DefineClickFunction("prevModel", []() {
        EventManager::notify(MENU_MODEL_PREV, nullptr);
    });

    DefineClickFunction("nextModel", []() {
        EventManager::notify(MENU_MODEL_NEXT, nullptr);
    });

    DefineClickFunction("nextPlayerModel", []() {
        EventManager::notify(MENU_MODEL_SELECT, nullptr);
    });

    DefineClickFunction("incrementPlayers", []() {
        UIComponent *data = UIManager::GetComponentById("dataPNum");
        int numPlayers = ++(data->size.x);

        TextComponent *players = dynamic_cast<TextComponent*>(UIManager::GetComponentById("playerNum"));
        if (players != nullptr)
            players->SetText(std::to_string(numPlayers));

        UIComponent *dec = UIManager::GetComponentById("decButton");
        dec->visible = true;

        if (numPlayers == 4) {
            UIComponent *inc = UIManager::GetComponentById("incButton");
            inc->visible = false;
        }
    });

    DefineClickFunction("decrementPlayers", []() {
        UIComponent *data = UIManager::GetComponentById("dataPNum");
        int numPlayers = --(data->size.x);

        TextComponent *players = dynamic_cast<TextComponent*>(UIManager::GetComponentById("playerNum"));
        if (players != nullptr)
            players->SetText(std::to_string(numPlayers));

        UIComponent *inc = UIManager::GetComponentById("incButton");
        inc->visible = true;

        if (numPlayers == 2) {
            UIComponent *dec = UIManager::GetComponentById("decButton");
            dec->visible = false;
        }
    });
}

UIComponent* UIManager::readChild(const XMLElement* element) {
    std::string type = element->Name();

    const XMLAttribute *offset = element->FindAttribute("offset"),
        *anchor = element->FindAttribute("anchor"),
        *visible = element->FindAttribute("visible"),
        *color = element->FindAttribute("color"),
        *alpha = element->FindAttribute("alpha"),
        *onclick = element->FindAttribute("onclick"),
        *ar = element->FindAttribute("ar"),
        *id = element->FindAttribute("id");

    float offX = 0, offY = 0, w = 0, h = 0;
    AnchorType aTypeX = ANCHOR_PERCENT, aTypeY = ANCHOR_PERCENT;
    VerticalAnchor vA = ANCHOR_TOP;
    HorizontalAnchor hA = ANCHOR_LEFT;
    glm::vec4 col = {0, 0, 0, 0};
    std::string idVal = "";
    bool vis = true;
    std::string clickFunc = "";
    float aspectRatio = 1.0f;

    if (visible != nullptr) {
        vis = visible->BoolValue();
    }

    if (id != nullptr) {
        idVal = id->Value();
    }

    if (onclick != nullptr) {
        clickFunc = onclick->Value();
    }

    if (ar != nullptr) {
        aspectRatio = ar->FloatValue();
    }

    if (anchor != nullptr) {
        std::istringstream a(anchor->Value());
        std::string val;

        a >> val;
        if (val == "center") {
            vA = ANCHOR_VCENTER;
        } else if (val == "bottom") {
            vA = ANCHOR_BOTTOM;
        }

        a >> val;
        if (val == "center") {
            hA = ANCHOR_HCENTER;
        } else if (val == "right") {
            hA = ANCHOR_RIGHT;
        }
    }

    if (offset != nullptr) {
        std::istringstream off(offset->Value());
        std::string val;

        off >> val;
        offX = std::stof(val);
        if (offX != 0 && val.back() == 'x') {
            aTypeX = ANCHOR_PIXEL;
        }

        off >> val;
        offY = std::stof(val);
        if (offY != 0 && val.back() == 'x') {
            aTypeY = ANCHOR_PIXEL;
        }
    }

    UIComponent *newComponent = nullptr;

    if (type == "component") {
        const XMLAttribute  *width = element->FindAttribute("width"),
            *height = element->FindAttribute("height");

        col = {0, 0, 0, 0};

        UnitType xUnit = UNIT_PERCENT, yUnit = UNIT_PERCENT;

        if (width != nullptr) {
            std::string val = width->Value();
            if (val == "scale") {
                xUnit = UNIT_SCALE;
            } else {
                w = std::stof(val);
                if (val.back() == 'x')
                    xUnit = UNIT_PIXEL;
            }
        }

        if (height != nullptr) {
            std::string val = height->Value();
            if (val == "scale") {
                yUnit = UNIT_SCALE;
            } else {
                h = std::stof(val);
                if (val.back() == 'x')
                    yUnit = UNIT_PIXEL;
            }
        }

        newComponent = new UIComponent(w, h, offX, offY);
        newComponent->xType = xUnit;
        newComponent->yType = yUnit;
        newComponent->aspectRatio = aspectRatio;
    } else if (type == "image") {
        const XMLAttribute  *width = element->FindAttribute("width"),
            *height = element->FindAttribute("height"),
            *src = element->FindAttribute("src");

        col = {1.0, 1.0, 1.0, 1.0};

        std::string srcString = "";
        UnitType xUnit = UNIT_PERCENT, yUnit = UNIT_PERCENT;

        if (width != nullptr) {
            std::string val = width->Value();
            if (val == "scale") {
                xUnit = UNIT_SCALE;
            } else {
                w = std::stof(val);
                if (val.back() == 'x')
                    xUnit = UNIT_PIXEL;
            }
        }

        if (height != nullptr) {
            std::string val = height->Value();
            if (val == "scale") {
                yUnit = UNIT_SCALE;
            } else {
                h = std::stof(val);
                if (val.back() == 'x')
                    yUnit = UNIT_PIXEL;
            }
        }

        if (src != nullptr) {
            srcString = "../UI/images/" + std::string(src->Value());
        }

        newComponent = new ImageComponent(srcString, w, h, offX, offY);
        newComponent->xType = xUnit;
        newComponent->yType = yUnit;
    } else if (type == "text") {
        const XMLAttribute *size = element->FindAttribute("size");
        const char *text = element->GetText();
        if (text == nullptr) {
            text = "";
        }

        col = {1.0, 1.0, 1.0, 1.0};

        UnitType sType = UNIT_PIXEL;
        float s = 0;
        if (size != nullptr) {
            std::string val = size->Value();
            s = std::stof(val);
            if (val.back() == '%')
                sType = UNIT_PERCENT;
        }

        newComponent = new TextComponent(text, s, offX, offY);
        newComponent->yType = sType;
    }

    if (color != nullptr) {
        std::istringstream cStream(color->Value());
        int c;
        cStream >> std::hex >> c;

        col.r = ((c >> 16) & 0xFF) / 255.0f;
        col.g = ((c >> 8) & 0xFF) / 255.0f;
        col.b = (c & 0xFF) / 255.0f;
        col.a = 1.0f;
    }

    if (alpha != nullptr) {
        col.a = std::stof(alpha->Value());
    }

    if (newComponent != nullptr) {
        newComponent->id = idVal;
        newComponent->anchorXType = aTypeX;
        newComponent->anchorYType = aTypeY;
        newComponent->vAnchor = vA;
        newComponent->hAnchor = hA;
        newComponent->visible = vis;
        newComponent->color = col;
        newComponent->ClickAction = clickFunc;
    }

    const XMLElement *child = element->FirstChildElement();
    while (child != nullptr) {
        UIComponent *childComponent = readChild(child);
        childComponent->parent = newComponent;
        newComponent->children.push_back(childComponent);
        child = child->NextSiblingElement();
    }

    return newComponent;
}
