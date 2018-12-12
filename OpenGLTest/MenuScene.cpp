#include "MenuScene.h"
#include "Planetoid.h"
#include "Map.h"

void MenuScene::InitScene() {
    UIComponent *menuUI = UIManager::GetComponentById("MenuScene");
    if (menuUI != nullptr)
        menuUI->visible = true;

    _models.push_back(AssetLoader::loadModel("../Models/Cat.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Cow.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Pug.obj"));
    _models.push_back(AssetLoader::loadModel("../Models/Slime.obj"));

    glm::vec4 playerColors[4] = {
        {0.447, 0.098, 0.745, 1.0}, 
        {0.922, 0.373, 0, 1.0},
        {0.047, 0.714, 0.604, 1.0},
        {0.922, 0.878, 0, 1.0},
    };

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
    cSkin->z = 0;
    cSkin->model = AssetLoader::loadModel("../Models/Cat.obj");
    cSkin->color = playerColors[0];
    cSkin->position = { mapWidth / 2.0f, mapHeight / 2.0f };
    cSkin->scale = glm::vec3(10.0f);
    c->setRenderable(cSkin);

    EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(c->renderable), false);
}

int MenuScene::Update(const float delta) {
	return -1;
}

void MenuScene::CleanUp() {
    UIComponent *menuUI = UIManager::GetComponentById("MenuScene");
    if (menuUI != nullptr)
        menuUI->visible = false;

    delete _background;
    delete c;
}