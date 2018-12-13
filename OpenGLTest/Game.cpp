#include "Game.h"
#include "MenuScene.h"
#include "MainScene.h"
#include <thread>

Game::Game() {
    _currScene = new MenuScene();
    _currScene->InitScene();

    EventManager::subscribe(GAME_START, this);
	EventManager::subscribe(GAME_END, this);
}

Game::~Game() {
    _currScene->CleanUp();
    delete _currScene;

	EventManager::unsubscribe(GAME_START, this);
	EventManager::unsubscribe(GAME_END, this);
}

void Game::notify(EventName eventName, Param *params) {
    switch (eventName) {
    case GAME_START: { // Transition to Main Scene on Start Game button press
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
	case GAME_END: { // Transition to Menu Scene at the end of the game
		UIComponent *comp = UIManager::GetComponentById("modelSelect");
		comp->visible = false;
		comp = UIManager::GetComponentById("splashScreen");
		comp->visible = true;
		comp = UIManager::GetComponentById("nextPlayerModel");
		comp->visible = true;
		comp = UIManager::GetComponentById("startGame");
		comp->visible = false;
		Transition(new MenuScene());
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
	// Control fade to black animation on separate thread
    std::thread t(&fadeOut, this);
    t.join();

	// Once black screen, clean up the current scene then initialize the next scene
    Scene *scene = _currScene;
    _currScene = nullptr;

	if (scene != nullptr)
	{
		scene->CleanUp();
		delete scene;
	}
    nextScene->InitScene();
    _currScene = nextScene;

	// After next scene is initialized, fade back in
    std::thread t2(&fadeIn, this);
    t2.join();
}

void Game::Update(const float delta) {
    if (_currScene != nullptr && !FadeOut && !FadeIn) {
		if (_currScene->Update(delta) != -1) // If scene update evalutes as someone has won the game, end game
			EventManager::notify(GAME_END, nullptr);
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