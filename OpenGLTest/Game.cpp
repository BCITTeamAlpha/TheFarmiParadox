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
    case GAME_START:
        Transition(new MainScene());
        break;
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
    scene->CleanUp();
    delete scene;
    nextScene->InitScene();
    _currScene = nextScene;

    std::thread t2(&fadeIn, this);
    t2.join();
}

void Game::Update(const float delta) {
    if (_currScene != nullptr && !FadeOut && !FadeIn) {
        _currScene->Update(delta);
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