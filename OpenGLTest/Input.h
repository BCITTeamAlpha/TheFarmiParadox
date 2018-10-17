#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <map>
#include <vector>

class Input {
public:

	using Callback = std::function<void()>;

	Input();
	~Input();

	void setInputCallbacks(GLFWwindow* window, GLFWkeyfun func, GLFWmousebuttonfun mouseFunc);

	void addKeyDownBinding(int key, const Callback& callback);

	void addKeyRepeatBinding(int key, const Callback& callback);

	void onKeyPress(int key);

	void onKeyRepeat(int key);

private:
	std::map<int, std::vector<Callback>> keypress_Callbacks;  //functions that are called on key down 
	std::map<int, std::vector<Callback>> keyrepeat_Callbacks; //functions that are called when the key is held down

};