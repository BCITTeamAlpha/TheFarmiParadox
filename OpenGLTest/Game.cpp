#include "Game.h"
#include "MenuScene.h"
#include "MainScene.h"
#include <thread>

Game::Game() {
    _currScene = new MenuScene();
    _currScene->InitScene();

    EventManager::subscribe(GAME_START, this);
}

Game::~Game() {
    _currScene->CleanUp();
    delete _currScene;
}

void Game::notify(EventName eventName, Param *params) {
    switch (eventName) {
    case GAME_START: {
        int numPlayer = 2;
        int models[] = { 0, 1, 2, 3 };

        TypeParam<std::pair<int, int*>> *p = dynamic_cast<TypeParam<std::pair<int,int*>>*>(params);
        if (p != nullptr) {
            numPlayer = p->Param.first;
            for (int i = 0; i < 4; i++)
                models[i] = p->Param.second[i];
            delete p->Param.second;
            delete params;
        }

        Transition(new MainScene(numPlayer, models));
        break;
    }
    default:
        break;
    }
}

void fadeOut(Game* game) {
    game->Black = UIManager::GetComponentById("BlackOverlay");
    game->FadeOut = true;
    while (game->FadeOut) {
        Sleep(1);
    }
}

void fadeIn(Game* game) {
    game->Black = UIManager::GetComponentById("BlackOverlay");
    game->FadeIn = true;
    while (game->FadeIn) {
        Sleep(1);
    }
}

void Game::Transition(Scene *nextScene) {
    std::thread t(&fadeOut, this);
    t.join();

    Scene *scene = _currScene;
    _currScene = nullptr;

	if (scene != nullptr)
	{
		scene->CleanUp();
		delete scene;
	}
    nextScene->InitScene();
    _currScene = nextScene;

    std::thread t2(&fadeIn, this);
    t2.join();
}

void Game::Update(const float delta) {
    if (_currScene != nullptr && !FadeOut && !FadeIn) {
		if (_currScene->Update(delta) != -1)
			Transition(new MenuScene());
    }
    if (FadeOut) {
        if (Black->color.a < 1)
            Black->color.a += 0.1;
        else {
            Black->color.a = 1;
            FadeOut = false;
        }
    } else if (FadeIn) {
        if (Black->color.a > 0)
            Black->color.a -= 0.1;
        else {
            Black->color.a = 0;
            FadeIn = false;
        }
    }
}