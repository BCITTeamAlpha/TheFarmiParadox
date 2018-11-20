#include "Renderer.h"

#include <algorithm>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "TextComponent.h"
#include "ImageComponent.h"

enum {
	UNIFORM_MODEL_MATRIX,
	UNIFORM_VIEW_MATRIX,
	UNIFORM_PROJECTION_MATRIX,
	UNIFORM_MATERIAL_COLOR,
	UNIFORM_MATERIAL_FULLBRIGHT,
	UNIFORM_MATERIAL_ROUGHNESS,
	UNIFORM_MATERIAL_METALLIC,
	UNIFORM_MATERIAL_F0,
	UNIFORM_LIGHT_COLOR,
	UNIFORM_LIGHT_BRIGHTNESS,
	UNIFORM_LIGHT_POSITION,
	UNIFORM_UI_VIEWPROJECTION_MATRIX,
	UNIFORM_UI_MODEL_MATRIX,
	UNIFORM_UI_MATERIAL_COLOR,
	NUM_UNIFORMS
};
GLuint uniforms[NUM_UNIFORMS];

void Renderer::DrawRenderable(Renderable* renderable) {
	glBindBuffer(GL_ARRAY_BUFFER, renderable->model.positionLoc);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, renderable->model.UVLoc);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, renderable->model.normalLoc);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable->model.elementLoc);

	glBindTexture(GL_TEXTURE_2D, renderable->texture.loc);

	glm::mat4 m = glm::mat4(1.0);
	m = glm::translate(m, renderable->getPosition3());
	m = glm::rotate(m, (*renderable->rotation).z * (float)M_PI / 180.0f, glm::vec3(0, 0, 1));
	m = glm::rotate(m, (*renderable->rotation).y * (float)M_PI / 180.0f, glm::vec3(0, 1, 0));
	m = glm::rotate(m, (*renderable->rotation).x * (float)M_PI / 180.0f, glm::vec3(1, 0, 0));
	m = glm::scale(m, renderable->scale);

	glUniformMatrix4fv(uniforms[UNIFORM_MODEL_MATRIX], 1, GL_FALSE, glm::value_ptr(m));
	glUniform4fv(uniforms[UNIFORM_MATERIAL_COLOR], 1, glm::value_ptr(glm::convertSRGBToLinear(renderable->color)));
	glUniform1i(uniforms[UNIFORM_MATERIAL_FULLBRIGHT], renderable->fullBright);
	glUniform1f(uniforms[UNIFORM_MATERIAL_ROUGHNESS], renderable->roughness);
	glUniform1f(uniforms[UNIFORM_MATERIAL_METALLIC], renderable->metallic);
	glUniform1f(uniforms[UNIFORM_MATERIAL_F0], renderable->f0);

	glDrawElements(GL_TRIANGLES, renderable->model.elements.size(), GL_UNSIGNED_INT, (void*)0);
}

void Renderer::DrawUIRenderable(Renderable* UIrenderable) {
	glBindBuffer(GL_ARRAY_BUFFER, UIrenderable->model.positionLoc);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, UIrenderable->model.UVLoc);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UIrenderable->model.elementLoc);

	glBindTexture(GL_TEXTURE_2D, UIrenderable->texture.loc);

	glm::mat4 m = glm::mat4(1.0);
	m = glm::translate(m, UIrenderable->getPosition3());
	m = glm::rotate(m, (*UIrenderable->rotation).z * (float)M_PI / 180.0f, glm::vec3(0, 0, 1));
	m = glm::rotate(m, (*UIrenderable->rotation).y * (float)M_PI / 180.0f, glm::vec3(0, 1, 0));
	m = glm::rotate(m, (*UIrenderable->rotation).x * (float)M_PI / 180.0f, glm::vec3(1, 0, 0));
	m = glm::scale(m, glm::vec3(1.0, 1.0, 1.0));

	glUniformMatrix4fv(uniforms[UNIFORM_UI_MODEL_MATRIX], 1, GL_FALSE, glm::value_ptr(m));
	glUniform4fv(uniforms[UNIFORM_UI_MATERIAL_COLOR], 1, glm::value_ptr(glm::convertSRGBToLinear(UIrenderable->color)));

	glDrawElements(GL_TRIANGLES, UIrenderable->model.elements.size(), GL_UNSIGNED_INT, (void*)0);
}

void Renderer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw 3d stuff
	glUseProgram(mainProgram);
	glm::mat4 v = glm::translate(glm::mat4(1.0), -cameraPosition);
	glm::mat4 p = glm::perspective(cameraFOV * (float)M_PI / 180.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, nearClip, farClip);
	glm::vec3 lightPosition = v * glm::vec4(cameraPosition.x, cameraPosition.y, 20.0, 1.0f);

	glUniformMatrix4fv(uniforms[UNIFORM_VIEW_MATRIX], 1, GL_FALSE, glm::value_ptr(v));
	glUniformMatrix4fv(uniforms[UNIFORM_PROJECTION_MATRIX], 1, GL_FALSE, glm::value_ptr(p));
	glUniform3fv(uniforms[UNIFORM_LIGHT_COLOR], 1, glm::value_ptr(glm::convertSRGBToLinear(glm::vec3(1.0f, 1.0f, 1.0f))));
	glUniform1f(uniforms[UNIFORM_LIGHT_BRIGHTNESS], 800.0f);
	glUniform3fv(uniforms[UNIFORM_LIGHT_POSITION], 1, glm::value_ptr(lightPosition));

	for (auto renderable : renderables) {
		DrawRenderable(renderable);
	}

	// draw 2d stuff
	glUseProgram(uiProgram);
	glm::mat4 ortho = glm::ortho(0.0f, (GLfloat)WIDTH, 0.0f, (GLfloat)HEIGHT, -100.0f, 100.0f);
	glUniformMatrix4fv(uniforms[UNIFORM_UI_VIEWPROJECTION_MATRIX], 1, GL_FALSE, glm::value_ptr(ortho));
    DrawUITree();
}

void Renderer::GenerateBuffers(Renderable * renderable) {
	glGenBuffers(1, &renderable->model.positionLoc);
	glGenBuffers(1, &renderable->model.UVLoc);
	glGenBuffers(1, &renderable->model.normalLoc);
	glGenBuffers(1, &renderable->model.elementLoc);
	glGenTextures(1, &renderable->texture.loc);
}

void Renderer::PopulateBuffers(Renderable * renderable) {
	glBindBuffer(GL_ARRAY_BUFFER, renderable->model.positionLoc);
	glBufferData(GL_ARRAY_BUFFER, renderable->model.positions.size() * sizeof(glm::vec3), renderable->model.positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, renderable->model.UVLoc);
	glBufferData(GL_ARRAY_BUFFER, renderable->model.UVs.size() * sizeof(glm::vec2), renderable->model.UVs.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, renderable->model.normalLoc);
	glBufferData(GL_ARRAY_BUFFER, renderable->model.normals.size() * sizeof(glm::vec3), renderable->model.normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable->model.elementLoc);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderable->model.elements.size() * sizeof(GLuint), renderable->model.elements.data(), GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, renderable->texture.loc);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, renderable->texture.width, renderable->texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, renderable->texture.data.data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Renderer::AddToRenderables(Renderable * renderable) {
    GenerateBuffers(renderable);
    PopulateBuffers(renderable);
    renderables.push_back(renderable);
}

void Renderer::AddToUIRenderables(UIComponent* renderable) {
    GenerateBuffers(renderable);
    PopulateBuffers(renderable);
}

void Renderer::CreateShaderProgram(GLuint &programLoc, const char* vertexShaderPath, const char* fragmentShaderPath) {
	//Create shader program
	programLoc = glCreateProgram();
	Shader *vShader = new Shader(vertexShaderPath, GL_VERTEX_SHADER);
	glAttachShader(programLoc, vShader->GetShader());
	delete(vShader);
	Shader *fShader = new Shader(fragmentShaderPath, GL_FRAGMENT_SHADER);
	glAttachShader(programLoc, fShader->GetShader());
	delete(fShader);
	glLinkProgram(programLoc);

	//Check for shader program linking errors
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(programLoc, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programLoc, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

int Renderer::RenderLoop() {
	std::unique_lock<std::mutex> lck(mtx);

	//Setup GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "The Farmi Paradox", nullptr, nullptr);

	//Ensure window was created
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	//Adjust width and height based on pixel density
	int scrnWidth, scrnHeight;
	glfwGetFramebufferSize(window, &scrnWidth, &scrnHeight);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	//Check that GLEW is initialized
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// spew OpenGL errors to stderr
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, NULL, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, 0x826b, 0, NULL, GL_FALSE);
	glDebugMessageCallback(MessageCallback, 0);
	glEnable(GL_DEBUG_OUTPUT);

	//Create the viewport
	glViewport(0, 0, scrnWidth, scrnHeight);

	CreateShaderProgram(mainProgram, "./VertexShader", "./FragmentShader");
	CreateShaderProgram(uiProgram, "./VertexShader2", "./FragmentShader2");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// uniforms for 3d stuff
	uniforms[UNIFORM_MODEL_MATRIX] = glGetUniformLocation(mainProgram, "model");
	uniforms[UNIFORM_VIEW_MATRIX] = glGetUniformLocation(mainProgram, "view");
	uniforms[UNIFORM_PROJECTION_MATRIX] = glGetUniformLocation(mainProgram, "projection");
	uniforms[UNIFORM_MATERIAL_FULLBRIGHT] = glGetUniformLocation(mainProgram, "u_fullBright");
	uniforms[UNIFORM_MATERIAL_COLOR] = glGetUniformLocation(mainProgram, "u_color");
	uniforms[UNIFORM_MATERIAL_ROUGHNESS] = glGetUniformLocation(mainProgram, "u_roughness");
	uniforms[UNIFORM_MATERIAL_METALLIC] = glGetUniformLocation(mainProgram, "u_metallic");
	uniforms[UNIFORM_MATERIAL_F0] = glGetUniformLocation(mainProgram, "u_f0");
	uniforms[UNIFORM_LIGHT_POSITION] = glGetUniformLocation(mainProgram, "u_lightPosition");
	uniforms[UNIFORM_LIGHT_BRIGHTNESS] = glGetUniformLocation(mainProgram, "u_lightBrightness");
	uniforms[UNIFORM_LIGHT_COLOR] = glGetUniformLocation(mainProgram, "u_lightColor");
	// uniforms for 2d stuff
	uniforms[UNIFORM_UI_MODEL_MATRIX] = glGetUniformLocation(uiProgram, "model");
	uniforms[UNIFORM_UI_VIEWPROJECTION_MATRIX] = glGetUniformLocation(uiProgram, "viewProjection");
	uniforms[UNIFORM_UI_MATERIAL_COLOR] = glGetUniformLocation(uiProgram, "u_color");

	// wireframe mode if we want to enable it for debugging
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// convert linear fragment shader output to srgb automatically
	glEnable(GL_FRAMEBUFFER_SRGB);

	// don't draw polygons if they are behind other polygons
	glEnable(GL_DEPTH_TEST);

	// don't draw polygons if they are facing away from the camera
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// use alpha for transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set opengl to swap framebuffer every # screen refreshes
	glfwSwapInterval(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	lck.unlock();
	cv.notify_all();

    uim = new UIManager(WIDTH, HEIGHT);

    UIComponent lBox(22.35, 100, 0, 0);
	lBox.id = "lBox";
	lBox.color = { 0.141, 0.427, 0.761, 1 };
    lBox.vAnchor = ANCHOR_TOP;
    lBox.hAnchor = ANCHOR_LEFT;

    UIComponent lBoxInner(90, 96, 0, 0);
	lBoxInner.id = "lBoxInner";
    lBoxInner.color = { 0.231,0.525,0.859,1};
    lBoxInner.vAnchor = ANCHOR_VCENTER;
    lBoxInner.hAnchor = ANCHOR_HCENTER;

    UIComponent rBox(21.9, 100, 0, 0);
	rBox.id = "rBox";
    rBox.color = { 0.141, 0.427, 0.761, 1 };
    rBox.vAnchor = ANCHOR_TOP;
    rBox.hAnchor = ANCHOR_RIGHT;

    UIComponent rBoxInner(90, 96, 0, 0);
	rBoxInner.id = "rBoxInner";
    rBoxInner.color = { 0.231,0.525,0.859,1 };
    rBoxInner.vAnchor = ANCHOR_VCENTER;
    rBoxInner.hAnchor = ANCHOR_HCENTER;

    TextComponent lText("T O P", 64, 0, 0);
	lText.id = "lText";
    lText.vAnchor = ANCHOR_TOP;
    lText.hAnchor = ANCHOR_HCENTER;
    lText.color = { 1,1,1,1 };

    TextComponent rText("K E K", 64, 0, 0);
	rText.id = "rText";
    rText.vAnchor = ANCHOR_TOP;
    rText.hAnchor = ANCHOR_HCENTER;
    rText.color = { 1,1,1,1 };

	UIComponent hideButton(90, 50, 0, 10);
	hideButton.id = "hideButton";
	hideButton.hAnchor = ANCHOR_HCENTER;
	hideButton.vAnchor = ANCHOR_BOTTOM;
	hideButton.color = {0.173, 0.184, 0.2, 1};
	hideButton.anchorYType = ANCHOR_PIXEL;
	hideButton.yType = UNIT_PIXEL;
	hideButton.ClickAction = []() {
		UIComponent* c = UIManager::GetComponentById("Karen");
		c->visible = !c->visible;
	};

	TextComponent btnTex("Hide", 40, 0, 0);
	btnTex.id = "btnTex";
	btnTex.vAnchor = ANCHOR_VCENTER;
	btnTex.hAnchor = ANCHOR_HCENTER;
	btnTex.color = {1,1,1,1};

    ImageComponent imageTest("./araragi_karen.png", 95, 0, 0, 0);
	imageTest.id = "Karen";
    imageTest.vAnchor = ANCHOR_BOTTOM;
    imageTest.hAnchor = ANCHOR_HCENTER;
    imageTest.xType = UNIT_PERCENT;
    imageTest.yType = UNIT_SCALE;

	hideButton.Add(&btnTex);

    lBoxInner.Add(&lText);
    lBoxInner.Add(&imageTest);
    rBoxInner.Add(&rText);
	rBoxInner.Add(&hideButton);

    lBox.Add(&lBoxInner);
    rBox.Add(&rBoxInner);

    uim->AddToRoot(&lBox);
    uim->AddToRoot(&rBox);

	while (!glfwWindowShouldClose(window)) {
		//Check for events like key pressed, mouse moves, etc.
		glfwPollEvents();

		while (renderables_waitList.size() != 0) {
			AddToRenderables(renderables_waitList.back());
			renderables_waitList.pop_back();
		}

		for (Renderable * renderable : renderables) {
			if (renderable->invalidated) {
				PopulateBuffers(renderable);
				renderable->invalidated = false;
			}
		}

		//draw
		draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Renderer::notify(EventName eventName, Param* params) {
    switch (eventName) {
		case RENDERER_ADD_TO_RENDERABLES: {
			TypeParam<std::shared_ptr<Renderable>> *p = dynamic_cast<TypeParam<std::shared_ptr<Renderable>> *>(params);
			renderables_waitList.push_back(p->Param.get());
			break;
		}
		case RENDERER_ADD_TO_UIRENDERABLES: {
			TypeParam<UIComponent*> *p = dynamic_cast<TypeParam<UIComponent*> *>(params);
			AddToUIRenderables(p->Param);
			break;
		}
		case RENDERER_POPULATE_BUFFERS: {
			TypeParam<UIComponent*> *p = dynamic_cast<TypeParam<UIComponent*> *>(params);
			PopulateBuffers(p->Param);
			break;
		}
		default:
			break;
    }
}

void Renderer::DrawUITree() {
    transparentList.clear();
    traverseChild(uim->Root());

    for (UIComponent *t : transparentList) {
        DrawUIRenderable(t);
    }
}

void Renderer::traverseChild(UIComponent *component) {
    if (component->IsTransparent()) {
        transparentList.push_back(component);
    } else {
        DrawUIRenderable(component);
    }

    for (UIComponent *child : component->children) {
        if (child->visible) {
            traverseChild(child);
        }
    }
}

Renderer::Renderer() {
	EventManager::subscribe(RENDERER_ADD_TO_RENDERABLES, this);
    EventManager::subscribe(RENDERER_ADD_TO_UIRENDERABLES, this);
    EventManager::subscribe(RENDERER_POPULATE_BUFFERS, this);
}

Renderer::~Renderer() {
    delete uim;
}