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

#include "shader.h"
#include "TextComponent.h"
#include "ImageComponent.h"

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

		glm::vec3 ambient_color = { 0.1, 0.1, 0.1 };

		glm::vec3 light_position = { 0.0f, 200.0f, 64.0f };
		glm::vec3 light_target = { 182.5f, 90.0f, 0.0f };
		glm::vec3 light_color = { 1.0f, 1.0f, 1.0f };
		GLfloat light_brightness = 40000.0f;
		GLfloat light_FOV = 60.0f;
		GLfloat light_nearclip = 0.1f;
		GLfloat	light_farclip = 1000.0f;
	private:
		void draw();

		void GenerateBuffers(std::shared_ptr<Renderable> renderable);
		void GenerateBuffers(UIComponent * renderable);
		void PopulateBuffers(std::shared_ptr<Renderable> renderable);
		void PopulateBuffers(UIComponent * renderable);

        void AddToRenderables(std::shared_ptr<Renderable> renderable);
        void AddToUIRenderables(UIComponent * renderable);

		void DrawRenderable(std::shared_ptr<Renderable> renderable);
		void DrawRenderable_ShadowMap(std::shared_ptr<Renderable> renderable);
        void DrawUIRenderable(UIComponent * renderable);

		void CreateShaderProgram(GLuint & programLoc, const char * vertexShaderPath, const char * fragmentShaderPath);

        void notify(EventName eventName, Param* params);    // Overrides ISubscriber::notify
        void DrawUITree();
        void traverseChild(UIComponent *component);

        std::list<UIComponent*> transparentList;

		std::mutex renderables_waitList_mutex;
		std::list<std::shared_ptr<Renderable>> renderables_waitList;
		std::list<std::shared_ptr<Renderable>> renderables;
		const GLuint WIDTH = 1280;
		const GLuint HEIGHT = 720;
		GLuint mainProgram, VAO;
		const GLuint SHADOW_WIDTH = 1024;
		const GLuint SHADOW_HEIGHT = 1024;
		GLuint shadowProgram, depthMap, depthMapFBO;
		GLuint uiProgram;

        UIManager *uim;
};
