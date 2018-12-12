#pragma once

class Scene {
public:  
    virtual void InitScene() = 0;
    virtual int Update(const float delta) = 0;
    virtual void CleanUp() = 0;
};

