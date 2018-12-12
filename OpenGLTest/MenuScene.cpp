#include "MenuScene.h"

void MenuScene::InitScene() {
    UIComponent *menuUI = UIManager::GetComponentById("MenuScene");
    if (menuUI != nullptr)
        menuUI->visible = true;
}

void MenuScene::Update(const float delta) {

}

void MenuScene::CleanUp() {
    UIComponent *menuUI = UIManager::GetComponentById("MenuScene");
    if (menuUI != nullptr)
        menuUI->visible = false;
}