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

    int NumPlayers = 2;
    int Winner = 0;
private:
    Scene *_currScene;
};