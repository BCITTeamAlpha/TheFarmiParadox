#pragma once
//uses UIComponents, Renderables
#include <list>
#include <mutex>

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "EventManager.h"
#include "Renderable.h"
#include "UIManager.h"

extern std::mutex mtx;
extern std::condition_variable cv;
extern GLFWwindow *window;

class Renderer : public ISubscriber {
	public:
		Renderer();
		int RenderLoop();
		~Renderer();

		glm::vec3 cameraPosition = { 64.0f, 64.0f, 64.0f };
		GLfloat cameraFOV = 90.0f;
		GLfloat nearClip = 0.1f;
		GLfloat farClip = 1000.0f;
	private:
		void draw();

		void GenerateBuffers(std::shared_ptr<Renderable> renderable);
		void GenerateBuffers(UIComponent * renderable);
		void PopulateBuffers(std::shared_ptr<Renderable> renderable);
		void PopulateBuffers(UIComponent * renderable);

        void AddToRenderables(std::shared_ptr<Renderable> renderable);
        void AddToUIRenderables(UIComponent * renderable);

		void DrawRenderable(std::shared_ptr<Renderable> renderable);
        void DrawUIRenderable(UIComponent * renderable);

		void CreateShaderProgram(GLuint & programLoc, const char * vertexShaderPath, const char * fragmentShaderPath);

        void notify(EventName eventName, Param* params);    // Overrides ISubscriber::notify
        void DrawUITree();
        void traverseChild(UIComponent *component);

        std::list<UIComponent*> transparentList;

		std::list<std::shared_ptr<Renderable>> renderables_waitList;
		std::list<std::shared_ptr<Renderable>> renderables;
		const GLuint WIDTH = 1280;
		const GLuint HEIGHT = 720;
		GLuint mainProgram, VAO;
		GLuint uiProgram;

        UIManager *uim;
};

