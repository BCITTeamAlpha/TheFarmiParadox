#pragma once

class Scene {
public:  
    virtual void InitScene() = 0;
    virtual void Update(const float delta) = 0;
    virtual void CleanUp() = 0;
};

