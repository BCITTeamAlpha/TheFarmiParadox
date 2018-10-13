#include "Renderer.h"

Renderable text;

void Renderer::DrawRenderable(Renderable* renderable) {
	glBindBuffer(GL_ARRAY_BUFFER, renderable->_positionBufferLocation);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, renderable->_texCoordBufferLocation);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, renderable->_normalBufferLocation);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable->_elementBufferLocation);

	glBindTexture(GL_TEXTURE_2D, renderable->_textureLocation);

	glm::mat4 m = glm::mat4(1.0);
	m = glm::translate(m, renderable->getPosition3());
	m = glm::rotate(m, (*renderable->_rotation).z * (float)M_PI / 180.0f, glm::vec3(0, 0, 1));
	m = glm::rotate(m, (*renderable->_rotation).y * (float)M_PI / 180.0f, glm::vec3(0, 1, 0));
	m = glm::rotate(m, (*renderable->_rotation).x * (float)M_PI / 180.0f, glm::vec3(1, 0, 0));
	m = glm::scale(m, glm::vec3(1.0, 1.0, 1.0));

	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(m));
	glUniform4fv(u_colorLoc, 1, glm::value_ptr(glm::convertSRGBToLinear(renderable->_color)));

	glDrawElements(GL_TRIANGLES, renderable->_elements.size(), GL_UNSIGNED_INT, (void*)0);
}

void Renderer::DrawUIRenderable(Renderable* renderable) {
	glBindBuffer(GL_ARRAY_BUFFER, renderable->_positionBufferLocation);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, renderable->_texCoordBufferLocation);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable->_elementBufferLocation);

	glBindTexture(GL_TEXTURE_2D, renderable->_textureLocation);

	glm::mat4 m = glm::mat4(1.0);
	m = glm::translate(m, renderable->getPosition3());
	m = glm::rotate(m, (*renderable->_rotation).z * (float)M_PI / 180.0f, glm::vec3(0, 0, 1));
	m = glm::rotate(m, (*renderable->_rotation).y * (float)M_PI / 180.0f, glm::vec3(0, 1, 0));
	m = glm::rotate(m, (*renderable->_rotation).x * (float)M_PI / 180.0f, glm::vec3(1, 0, 0));
	m = glm::scale(m, glm::vec3(1.0, 1.0, 1.0));

	glUniformMatrix4fv(mLocUI, 1, GL_FALSE, glm::value_ptr(m));
	glUniform4fv(u_colorLocUI, 1, glm::value_ptr(glm::convertSRGBToLinear(renderable->_color)));

	glDrawElements(GL_TRIANGLES, renderable->_elements.size(), GL_UNSIGNED_INT, (void*)0);
}

void Renderer::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw 3d stuff
	glUseProgram(mainProgram);
	glm::mat4 v = glm::translate(glm::mat4(1.0), -cameraPosition);
	glm::mat4 p = glm::perspective(cameraFOV * (float)M_PI / 180.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, nearClip, farClip);
	glm::vec3 lightPosition = v * glm::vec4(cameraPosition.x, cameraPosition.y, 10.0f, 1.0f);
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(v));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(p));
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPosition));
	for (auto renderable : renderables) {
		DrawRenderable(renderable);
	}

	// draw 2d stuff
	glUseProgram(uiProgram);
	glm::mat4 ortho = glm::ortho(0.0f, (GLfloat)WIDTH, 0.0f, (GLfloat)HEIGHT, -100.0f, 100.0f);
	glUniformMatrix4fv(vpLocUI, 1, GL_FALSE, glm::value_ptr(ortho));
    traverseUITree(uim->_root);
    DrawUIRenderable(&text);
}

void Renderer::GenerateBuffers(Renderable &renderable) {
	glGenBuffers(1, &renderable._positionBufferLocation);
	glGenBuffers(1, &renderable._texCoordBufferLocation);
	glGenBuffers(1, &renderable._normalBufferLocation);
	glGenBuffers(1, &renderable._elementBufferLocation);
}

void Renderer::PopulateBuffers(Renderable &renderable) {
	glBindBuffer(GL_ARRAY_BUFFER, renderable._positionBufferLocation);
	glBufferData(GL_ARRAY_BUFFER, renderable._positions.size() * sizeof(glm::vec3), renderable._positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, renderable._texCoordBufferLocation);
	glBufferData(GL_ARRAY_BUFFER, renderable._texCoords.size() * sizeof(glm::vec2), renderable._texCoords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, renderable._normalBufferLocation);
	glBufferData(GL_ARRAY_BUFFER, renderable._normals.size() * sizeof(glm::vec3), renderable._normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable._elementBufferLocation);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderable._elements.size() * sizeof(GLuint), renderable._elements.data(), GL_STATIC_DRAW);
}

void Renderer::AddToRenderables(Renderable& renderable) {
    GenerateBuffers(renderable);
    PopulateBuffers(renderable);
    renderables.push_back(&renderable);
}

void Renderer::AddToUIRenderables(UIComponent* renderable) {
    GenerateBuffers(*renderable);
    PopulateBuffers(*renderable);
    //uiRenderables.push_back(renderable);
    std::cout << "Added UI Renderable" << std::endl;
}

void Renderer::RemoveFromRenderables(Renderable& renderable) {
	glDeleteBuffers(1, &renderable._positionBufferLocation);
	glDeleteBuffers(1, &renderable._texCoordBufferLocation);
	glDeleteBuffers(1, &renderable._normalBufferLocation);
	glDeleteBuffers(1, &renderable._elementBufferLocation);
	renderables.erase(remove(renderables.begin(), renderables.end(), &renderable), renderables.end());
}

GLuint createTexture(const char* filename) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "Failed to load texture" << std::endl;
		return 0;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (nrChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	} else if (nrChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	std::cout << "Successfully loaded texture" << std::endl;
	return texture;
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

glm::vec2 charToVec2(char c) {
	int c_value = int(c);

	int index = 47; // default to ' '
	if (c_value>64 && c_value<91) // A-Z
		index = c_value - 65;
	else if (c_value>96 && c_value<123) // a-z
		index = c_value - 97;
	else if (c_value>47 && c_value<58) // 0-9
		index = c_value - 22;
	else if (c_value == 33) // !
		index = 36;
	else if (c_value == 63) // ?
		index = 37;
	else if (c_value == 43) // +
		index = 38;
	else if (c_value == 45) // -
		index = 39;
	else if (c_value == 61) // =
		index = 40;
	else if (c_value == 58) // :
		index = 41;
	else if (c_value == 46) // .
		index = 42;
	else if (c_value == 44) // ,
		index = 43;
	else if (c_value == 42) // *
		index = 44;
	else if (c_value == 36) // $
		index = 45;

	glm::vec2 ret;
	ret.x = 0.0f + 0.125f * (index % 8);
	ret.y = 1.0f - 0.125f * (index / 8);
	return ret;
}

int Renderer::RenderLoop(Renderable **pp) {
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

	//Create the viewport
	glViewport(0, 0, scrnWidth, scrnHeight);

	CreateShaderProgram(mainProgram, "./VertexShader", "./FragmentShader");
	CreateShaderProgram(uiProgram, "./VertexShader2", "./FragmentShader2");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	mLoc = glGetUniformLocation(mainProgram, "model");
	vLoc = glGetUniformLocation(mainProgram, "view");
	pLoc = glGetUniformLocation(mainProgram, "projection");
	u_colorLoc = glGetUniformLocation(mainProgram, "u_color");
	lightPosLoc = glGetUniformLocation(mainProgram, "lightPosition");

	mLocUI = glGetUniformLocation(uiProgram, "model");
	vpLocUI = glGetUniformLocation(uiProgram, "viewProjection");
	u_colorLocUI = glGetUniformLocation(uiProgram, "u_color");

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
	glClearColor(0.025f, 0.025f, 0.019f, 1.0f);

	glm::vec2 pos = glm::vec2(0, 0);
	glm::vec3 rot = glm::vec3(0, 0, 0);
	text._position = &pos;
	text._rotation = &rot;
    text._z = 1;
	std::string string = "Text rendering kind of working?";
	for (int i = 0; i < string.size(); i++) {
		glm::vec2 UV_topLeft = charToVec2(string[i]);
		UV_topLeft;
		text._positions.push_back(glm::vec3(32 * i, 32, 0)); // top-left
		text._positions.push_back(glm::vec3(32 * i + 32, 32, 0)); //top-right
		text._positions.push_back(glm::vec3(32 * i, 0, 0)); // bottom-left
		text._positions.push_back(glm::vec3(32 * i + 32, 0, 0)); // bottom-right
		text._texCoords.push_back(UV_topLeft);
		text._texCoords.push_back(UV_topLeft + glm::vec2(0.125, 0));
		text._texCoords.push_back(UV_topLeft + glm::vec2(0, -0.125));
		text._texCoords.push_back(UV_topLeft + glm::vec2(0.125, -0.125));
		text._elements.push_back(i * 4 + 1);
		text._elements.push_back(i * 4 + 0);
		text._elements.push_back(i * 4 + 2);
		text._elements.push_back(i * 4 + 1);
		text._elements.push_back(i * 4 + 2);
		text._elements.push_back(i * 4 + 3);
	}
	text._color = glm::vec4(1, 1, 1, 1);
	text._textureLocation = createTexture("./font.png");
	GenerateBuffers(text);
	PopulateBuffers(text);

    uim = new UIManager(WIDTH, HEIGHT);

    UIComponent centerBox(50, 50, 0, 0);
    centerBox._color = { 1,1,1,1 };
    centerBox.hAnchor = ANCHOR_HCENTER;
    centerBox.vAnchor = ANCHOR_VCENTER;

    UIComponent centerBox2(50, 50, 0, 0);
    centerBox2._color = { 0,0,1,0.2 };
    centerBox2.hAnchor = ANCHOR_HCENTER;
    centerBox2.vAnchor = ANCHOR_VCENTER;
    
    //centerBox.Add(&centerBox2);
    uim->AddToRoot(&centerBox2);

	while (!glfwWindowShouldClose(window)) {
		//Check for events like key pressed, mouse moves, etc.
		glfwPollEvents();

		if (*pp != NULL) {
			AddToRenderables(**pp);
			*pp = NULL;
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
    case RENDERER_ADD_TO_UIRENDERABLES: {
        TypeParam<UIComponent*> *p = dynamic_cast<TypeParam<UIComponent*> *>(params);
        AddToUIRenderables(p->Param);
        break;
    }
    default:
        break;
    }
}

void Renderer::traverseUITree(UIComponent *component) {
    DrawUIRenderable(component);

    for (UIComponent *child : component->children) {
        if (child->visible) {
            traverseUITree(child);
        }
    }
}


Renderer::Renderer() {
    EventManager::subscribe(RENDERER_ADD_TO_UIRENDERABLES, this);
}

Renderer::~Renderer() {
    delete uim;
}