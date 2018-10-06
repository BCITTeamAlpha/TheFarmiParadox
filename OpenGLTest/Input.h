#pragma once
#include <GL/glew.h>
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

	void addKeyDownBinding(int key, const Callback& callback)
	{
		keypress_Callbacks[key].push_back(callback);
	}

	void addKeyRepeatBinding(int key, const Callback& callback) {
		keyrepeat_Callbacks[key].push_back(callback);
	}

	void onKeyPress(int key)
	{
		for (Callback& callback : keypress_Callbacks[key])
		{
			callback();
		}
	}

	void onKeyRepeat(int key) {
		for (Callback& callback : keyrepeat_Callbacks[key])
		{
			callback();
		}
	}
	
private:
	std::map<int, std::vector<Callback>> keypress_Callbacks;  //functions that are called on key down 
	std::map<int, std::vector<Callback>> keyrepeat_Callbacks; //functions that are called when the key is held down

};