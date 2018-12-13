#pragma once

#include "Scene.h"
#include "EventManager.h"
#include "UIManager.h"


/**
Game contains scenes for game and transitions between them
*/
class Game : public ISubscriber {
public:
	Game();
    ~Game();

	// Update function called each frame passing in delta time for that frame
    void Update(const float delta);

	// Transition the current scene to the given scene. Cleanup current scene and initialize and execute the new scene 
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