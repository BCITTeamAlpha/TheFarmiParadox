#include "UIManager.h"

UIManager::UIManager(float width, float height) : 
    _root(nullptr, 100, 100, 0, 0), _screenSize(width, height) {
    _root.screenPosition = glm::vec2(0, 0);
    _root.screenSize = _screenSize;
}