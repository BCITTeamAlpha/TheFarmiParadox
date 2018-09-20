#include "Renderer.h"

std::thread renderThread;
const GLint WIDTH = 1280, HEIGHT = 720;

std::vector<glm::vec3> quadVertices = { { 0, 5, 0 },{ 5, 5, 0 },{ 0, 0, 0 },{ 5, 0, 0 } };
std::vector<glm::vec4> quadColors = { { 1, 0, 0, 1 },{ 1, 0, 0, 1 },{ 1, 0, 0, 1 },{ 1, 0, 0, 1 } };
std::vector<glm::vec3> quadNormals = { { 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 },{ 0, 0, 1 } };
std::vector<GLuint> quadElements = { 0, 1, 2, 2, 1, 3 };

GLuint mainProgram, VAO;

std::vector<glm::vec3> vertexVector;
std::vector<glm::vec4> colorVector;
std::vector<glm::vec3> normalVector;
std::vector<GLuint> indexVector;

std::list<IRenderable*> renderables;

GLuint vertexBuffer, colorBuffer, normalBuffer, elementBuffer;

// camera variables
// TODO: extract camera into its own class
glm::vec3 cameraPosition = { 0.0f, 0.0f, 10.0f };
float cameraFOV = 90.0f, nearClip = 0.1f, farClip = 100.0f;

glm::vec3 quadPosition = { 64.0f, 32.0f, 0.0f };

IRenderable i;

void AddToRenderables(IRenderable& renderable) {
	glGenBuffers(1, &renderable._vertexBufferLocation);
	glBindBuffer(GL_ARRAY_BUFFER, renderable._vertexBufferLocation);
	glBufferData(GL_ARRAY_BUFFER, renderable._vertices.size() * sizeof(glm::vec3), renderable._vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &renderable._colorBufferLocation);
	glBindBuffer(GL_ARRAY_BUFFER, renderable._colorBufferLocation);
	glBufferData(GL_ARRAY_BUFFER, renderable._colors.size() * sizeof(glm::vec4), renderable._colors.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &renderable._normalBufferLocation);
	glBindBuffer(GL_ARRAY_BUFFER, renderable._normalBufferLocation);
	glBufferData(GL_ARRAY_BUFFER, renderable._normals.size() * sizeof(glm::vec3), renderable._normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &renderable._elementBufferLocation);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable._elementBufferLocation);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderable._elements.size() * sizeof(GLuint), renderable._elements.data(), GL_STATIC_DRAW);

	renderables.push_back(&renderable);
}

void PopulateVectors() {
	Map m = Map(0);

	// center camera relative to map
	cameraPosition.x = (m.width() - 1) * 0.5f;
	cameraPosition.y = (m.height() - 1) * 0.5f;

	// move camera far enough that the entire map is visible
	// assumes window is wider than tall
	cameraPosition.z = (m.height() - 1) * 0.5f / std::tan(cameraFOV * M_PI / 360.0f);

	m.explosion(Planetoid(55.0f, 55.0f, 8.0f));
	vertexVector = MarchingSquares::GenerateMesh(m);
	for (GLuint i = 0; i < vertexVector.size(); i++) {
		colorVector.push_back(glm::vec4(0, 1, 0, 1));
		normalVector.push_back(glm::vec3(0, 0, 1));
		indexVector.push_back(i);
	}
}

void DrawRenderable(IRenderable& renderable) {
	glBindBuffer(GL_ARRAY_BUFFER, renderable._vertexBufferLocation);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, renderable._colorBufferLocation);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, renderable._normalBufferLocation);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable._elementBufferLocation);
	glDrawElements(GL_TRIANGLES, renderable._elements.size(), GL_UNSIGNED_INT, (void*)0);
}

void DrawBuffers(GLuint v, GLuint c, GLuint n, GLuint e, GLuint count) {
	glBindBuffer(GL_ARRAY_BUFFER, v);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, c);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, n);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mainProgram);

	glm::mat4 m;
	glm::mat4 v = glm::translate(glm::mat4(1.0), -cameraPosition);
	glm::mat4 p = glm::perspective(cameraFOV * (float)M_PI / 180.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, nearClip, farClip);

	GLint mvLoc = glGetUniformLocation(mainProgram, "modelView");
	GLint pLoc = glGetUniformLocation(mainProgram, "projection");
	GLint lightPositionLoc = glGetUniformLocation(mainProgram, "lightPosition");

	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(p));

	glm::vec3 lightPosition = v * glm::vec4(cameraPosition.x, cameraPosition.y, 10.0f, 1.0f);
	glUniform3fv(lightPositionLoc, 1, glm::value_ptr(lightPosition));

	m = glm::mat4(1.0);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(v * m));
	DrawBuffers(vertexBuffer, colorBuffer, normalBuffer, elementBuffer, indexVector.size());

	for (auto renderable : renderables) {
		DrawRenderable(*renderable);
	}

	m = glm::translate(glm::mat4(1.0), quadPosition);
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(v * m));
}

int notMain() {
	PopulateVectors();

	//Setup GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "The Farmi Paradox", nullptr, nullptr);

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

	//Load vertex shader
	Shader *vShader = new Shader("./VertexShader", GL_VERTEX_SHADER);

	//Load fragment shader
	Shader *fShader = new Shader("./FragmentShader", GL_FRAGMENT_SHADER);

	//Link shaders to program
	mainProgram = glCreateProgram();
	glAttachShader(mainProgram, vShader->GetShader());
	glAttachShader(mainProgram, fShader->GetShader());
	glLinkProgram(mainProgram);

	//Check for linking errors
	GLint success;
	GLchar infoLog[512];

	glGetProgramiv(mainProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(mainProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	delete(vShader);
	delete(fShader);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &colorBuffer);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &elementBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(glm::vec3), vertexVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colorVector.size() * sizeof(glm::vec4), colorVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normalVector.size() * sizeof(glm::vec3), normalVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexVector.size() * sizeof(unsigned int), indexVector.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	i._vertices = quadVertices;
	i._colors = quadColors;
	i._normals = quadNormals;
	i._elements = quadElements;
	AddToRenderables(i);

	// wireframe mode if we want to enable it for debugging
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// convert linear fragment shader output to srgb automatically
	glEnable(GL_FRAMEBUFFER_SRGB);

	// set opengl to swap framebuffer every # screen refreshes
	glfwSwapInterval(1);

	glClearColor(0.025f, 0.025f, 0.019f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		//Check for events like key pressed, mouse moves, etc.
		glfwPollEvents();

		//draw
		draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	std::terminate();
}

Renderer::Renderer() {
	renderThread = std::thread(notMain);
}


Renderer::~Renderer()
{
}