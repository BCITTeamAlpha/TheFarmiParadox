#include "Input.h"


Input::Input() {

}

Input::~Input() {

}

void Input::setInputCallbacks(GLFWwindow* window, GLFWkeyfun func, GLFWmousebuttonfun mouseFunc) {
	glfwSetKeyCallback(window, func);
	glfwSetMouseButtonCallback(window, mouseFunc);
}

void Input::addKeyDownBinding(int key, const Callback& callback)
{
	keypress_Callbacks[key].push_back(callback);
}

void Input::addKeyRepeatBinding(int key, const Callback& callback) {
	keyrepeat_Callbacks[key].push_back(callback);
}

void Input::onKeyPress(int key)
{
	for (Callback& callback : keypress_Callbacks[key])
	{
		callback();
	}
}

void Input::onKeyRepeat(int key) {
	for (Callback& callback : keyrepeat_Callbacks[key])
	{
		callback();
	}
}