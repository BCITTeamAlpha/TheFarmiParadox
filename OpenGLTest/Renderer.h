#pragma once

#include <iostream>
#include <list>
#define _USE_MATH_DEFINES
#include <Math.h>
#include <mutex>
#include <thread>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "IRenderable.h"
#include "shader.h"

class Renderer {
	public:
		Renderer(IRenderable ***ppp, std::mutex &mtx);
		~Renderer();
};

