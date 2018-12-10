#pragma once

#include "Scene.h"
#include "UIManager.h"

class MenuScene : public Scene {
public:
    void InitScene();
    void Update(const float delta);
    void CleanUp();
private:
};

