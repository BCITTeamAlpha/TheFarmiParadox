#include <vector>
#include <Windows.h>

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <glm/glm.hpp>

#include "AssetLoader.h"
#include "MarchingSquares.h"
#include "Renderer.h"
#include "Renderable.h"
#include "Character.h"
#include "Input.h"
#include "SoundManager.h"
#include "PlayerManager.h"
#include <thread>
#include "Player.h"
#include "Game.h"

GLFWwindow* window;
std::mutex mtx;
std::condition_variable cv;

Input inputHandler;
//If we want to bind a key directly to a function
//inputHandler.addKeyDownBinding(GLFW_KEY_WHATEVER, Class::func or class.func);
double xpos, ypos;

Renderer *renderer;
SoundManager* noise;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_LEFT:
	case GLFW_KEY_A:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);  	
			EventManager::notify(PLAYER_LEFT, &param, false);
			EventManager::notify(AIM_LEFT, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(PLAYER_LEFT, &param, false);
			EventManager::notify(AIM_LEFT, &param, false);
		}
		break;
	case GLFW_KEY_RIGHT:
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);
			EventManager::notify(PLAYER_RIGHT, &param, false);
			EventManager::notify(AIM_RIGHT, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(PLAYER_RIGHT, &param, false);
			EventManager::notify(AIM_RIGHT, &param, false);
		}
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			TypeParam<bool> param(true);
			EventManager::notify(PLAYER_JUMP, &param, false);
		}
		if (action == GLFW_RELEASE)
		{
			TypeParam<bool> param(false);
			EventManager::notify(PLAYER_JUMP, &param, false);
            
		}
		break;
	case GLFW_KEY_F:
		if (action == GLFW_PRESS)
		{
			EventManager::notify(PLAYER_FIRE, nullptr, false);
		}
	default:
		if (action == GLFW_PRESS) inputHandler.onKeyPress(key);
		if (action == GLFW_REPEAT) inputHandler.onKeyRepeat(key);

		break;
	}

}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	glfwGetCursorPos(window, &xpos, &ypos);
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) //GLFW_RELEASE is the other possible state.
	{
		//printf("%lf %lf\n", xpos, ypos);
        EventManager::notify(BULLET_SPAWN, nullptr);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) //GLFW_RELEASE is the other possible state.
	{
		//printf("Right mouse button released\n");
		//playerManager->UpdatePlayerUI();
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) //GLFW_RELEASE is the other possible state.
	{
		TypeParam<std::pair<float, float>> param(std::pair<float,float>(xpos, ypos));
		EventManager::notify(UI_CLICK, &param, false);
		printf("left mouse button clicked at: ");
		printf("%lf %lf\n", xpos, ypos);
	}
}


int main() {
	// start Renderer in own thread
	renderer = new Renderer();
	std::unique_lock<std::mutex> lck(mtx);
	std::thread renderThread = std::thread(&Renderer::RenderLoop, renderer);

	renderer->cameraFOV = 70;

    Game game = Game();

	//Set input handling callbacks
	cv.wait(lck);
	inputHandler.setInputCallbacks(window, KeyCallback, mouse_button_callback);
	
	//TESTING FOR THE INVENTORY/WEAPON SYSTEM
	inputHandler.addKeyDownBinding(GLFW_KEY_Q, PlayerManager::prevWeapon);
	inputHandler.addKeyDownBinding(GLFW_KEY_E, PlayerManager::nextWeapon);

    //adding sound
    noise = new SoundManager();
    EventManager::subscribe(PLAY_SONG, noise);
    EventManager::subscribe(PLAY_SOUND, noise);
    EventManager::subscribe(GAME_START, noise);

    //start initial music track
    TrackParams * initial = new TrackParams();

    initial->track = MenuBGM;

    initial->x = 0;
    initial->y = 0;
    initial->z = 0;

    TypeParam<TrackParams*> param(initial);
    EventManager::notify(PLAY_SONG, &param);

	for (int tick = 0;; tick++)
	{
        game.Update(1.0 / 59.94);
		Sleep(1000.0 / 59.94);

		// code to move light around
		float x = 85.0f + 256.0f * std::sin(tick * 0.01f);
		float y = 100.0f + 256.0f * std::cos(tick * 0.01f);
		renderer->light_position.x = x;
		renderer->light_position.y = y;
	}
}