#pragma once

#include "Scene.h"
#include "EventManager.h"
#include "UIManager.h"

class Game : public ISubscriber {
public:
	Game();
    ~Game();

    void Update(const float delta);
    void Transition(Scene *nextScene);

    void notify(EventName eventName, Param *params); // overrides ISubscriber::notify

    bool FadeOut = false;
    bool FadeIn = false;
    UIComponent *Black = nullptr;
private:
    Scene *_currScene;
};