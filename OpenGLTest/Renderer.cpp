#include "Renderer.h"

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
	m = glm::translate(m, *renderable->_position);
	m = glm::rotate(m, (*renderable->_rotation).z * (float)M_PI / 180.0f, glm::vec3(0, 0, 1));
	m = glm::rotate(m, (*renderable->_rotation).y * (float)M_PI / 180.0f, glm::vec3(0, 1, 0));
	m = glm::rotate(m, (*renderable->_rotation).x * (float)M_PI / 180.0f, glm::vec3(1, 0, 0));
	m = glm::scale(m, glm::vec3(1.0, 1.0, 1.0));

	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(m));
	glUniform4fv(u_colorLoc, 1, glm::value_ptr(renderable->_color));

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

void Renderer::RemoveFromRenderables(Renderable& renderable) {
	glDeleteBuffers(1, &renderable._positionBufferLocation);
	glDeleteBuffers(1, &renderable._texCoordBufferLocation);
	glDeleteBuffers(1, &renderable._normalBufferLocation);
	glDeleteBuffers(1, &renderable._elementBufferLocation);
	renderables.erase(remove(renderables.begin(), renderables.end(), &renderable), renderables.end());
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

Renderer::Renderer() {
}

Renderer::~Renderer() {
}