#include "MenuScene.h"
#include "Planetoid.h"
#include "Map.h"
#include "TextComponent.h"

void MenuScene::InitScene() {
    EventManager::subscribe(MENU_MODEL_NEXT, this);
    EventManager::subscribe(MENU_MODEL_PREV, this);
    EventManager::subscribe(MENU_MODEL_SELECT, this);
    EventManager::subscribe(MENU_SHOW_MODEL, this);

    UIComponent *menuUI = UIManager::GetComponentById("MenuScene");
    if (menuUI != nullptr)
        menuUI->visible = true;

    _models.push_back(AssetLoader::loadModel("../Models/Cat.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Chick.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Cow.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Dolphin.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Dragon.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Horse.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Llama.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Pig.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Dog.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Pug.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Red Fox.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Sheep.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Whale.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Wolf.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Slime.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Zebra.obj"));

    _currModel = 0;

    int mapWidth = 362, mapHeight = 205;

    glm::vec3 cameraPos = { mapWidth / 2.0f,
                       mapHeight / 2.0f,
                       mapHeight / 2.0f };
    TypeParam<glm::vec3> param(cameraPos);
    EventManager::notify(RENDERER_SET_CAMERA, &param);

    // setup background
    Renderable* backgroundSkin = new Renderable();
    _background = new GameObject();
    _background->setRenderable(backgroundSkin);
    backgroundSkin->z = -10;
    backgroundSkin->position = glm::vec2(mapWidth / 2.0f, mapHeight / 2.0f);
    backgroundSkin->scale.y = (cameraPos.z - backgroundSkin->z) * 2;
    backgroundSkin->scale.x = backgroundSkin->scale.y * (float)mapWidth / (float)mapHeight;
    backgroundSkin->model = AssetLoader::loadModel("quad.obj");
    backgroundSkin->texture.width = mapWidth;
    backgroundSkin->texture.height = mapHeight;
    GLubyte* backgroundImage = new GLubyte[backgroundSkin->texture.width * backgroundSkin->texture.height * 4];
    for (int x = 0; x < backgroundSkin->texture.width; x++) {
        for (int y = 0; y < backgroundSkin->texture.height; y++) {
            size_t index = (x + y * backgroundSkin->texture.width) * 4;

            backgroundImage[index] = 63;
            backgroundImage[index + 1] = 127;
            backgroundImage[index + 2] = 255;
            backgroundImage[index + 3] = 0;

            if (rand() % 255 == 0 && backgroundImage[index + 3] == 0) {
                backgroundImage[index] = 255 - backgroundImage[index + 3];
                backgroundImage[index + 1] = 255 - backgroundImage[index + 3];
                backgroundImage[index + 2] = 255 - backgroundImage[index + 3];
                backgroundImage[index + 3] = 255;
            }
        }
    }
    backgroundSkin->texture.data.assign((GLubyte*)backgroundImage, (GLubyte*)backgroundImage + backgroundSkin->texture.width * backgroundSkin->texture.height * 4);
    delete backgroundImage;
    backgroundSkin->fullBright = true;

    EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(_background->renderable), false);
    
    c = new GameObject();
    Renderable *cSkin = new Renderable();
    c->setRenderable(cSkin);
    cSkin->z = 0;
    cSkin->model = _models[_currModel];
    cSkin->color = playerColors[0];
    cSkin->position = {-mapWidth / 2.0f, -mapHeight / 2.0f };
    cSkin->scale = glm::vec3(10.0f);
    cSkin->fullBright = true;

    EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(c->renderable), false);
}

void MenuScene::Update(const float delta) {
    c->set_rotation(c->get_rotation() + glm::vec3({0, 1, 0}));
    c->renderable->color = playerColors[_currPlayer];
}

void MenuScene::CleanUp() {
    UIComponent *menuUI = UIManager::GetComponentById("MenuScene");
    if (menuUI != nullptr)
        menuUI->visible = false;

    delete _background;
    delete c;
}

void MenuScene::notify(EventName eventName, Param *params) {
    switch (eventName) {
    case MENU_MODEL_NEXT: {
        _currModel++;
        if (_currModel >= _models.size())
            _currModel = 0;
        UIComponent *pModel = UIManager::GetComponentById("dataP" + std::to_string(_currPlayer + 1) + "Model");
        pModel->size.x = _currModel;
        c->renderable->model.positions = _models[_currModel].positions;
        c->renderable->model.normals = _models[_currModel].normals;
        c->renderable->model.UVs = _models[_currModel].UVs;
        c->renderable->model.elements = _models[_currModel].elements;
        c->renderable->invalidated = true;
        break;
    }
    case MENU_MODEL_PREV: {
        _currModel--;
        if (_currModel < 0)
            _currModel = _models.size() - 1;
        UIComponent *pModel = UIManager::GetComponentById("dataP" + std::to_string(_currPlayer + 1) + "Model");
        pModel->size.x = _currModel;
        c->renderable->model.positions = _models[_currModel].positions;
        c->renderable->model.normals = _models[_currModel].normals;
        c->renderable->model.UVs = _models[_currModel].UVs;
        c->renderable->model.elements = _models[_currModel].elements;
        c->renderable->invalidated = true;
        break;
    }
    case MENU_MODEL_SELECT: {
        _currPlayer++;
        UIComponent *data = UIManager::GetComponentById("dataPNum");
        int numPlayers = data->size.x;
        TextComponent *playerString = dynamic_cast<TextComponent*>(UIManager::GetComponentById("selectAnimal"));
        playerString->SetText("Select Player " + std::to_string(_currPlayer + 1) + "'s Animal");
        if (_currPlayer == numPlayers - 1) {
            UIComponent *btn = UIManager::GetComponentById("nextPlayerModel");
            btn->visible = false;
            btn = UIManager::GetComponentById("startGame");
            btn->visible = true;
        }
        break;
    }
    case MENU_SHOW_MODEL: {
        c->renderable->position *= -1;
        break;
    }
    default:
        break;
    }
}