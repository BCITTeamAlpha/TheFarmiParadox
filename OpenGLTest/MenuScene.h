#pragma once

#include "Scene.h"
#include "UIManager.h"
#include "GameObject.h"
#include "Renderable.h"
#include "AssetLoader.h"
#include <vector>

class MenuScene : public Scene {
public:
    void InitScene();
    int Update(const float delta);
    void CleanUp();
private:
    GameObject *_background;
    std::vector<Model> _models;
    GameObject *c;
};

