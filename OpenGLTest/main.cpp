#include "openglstuff.h"
#include "shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

const GLfloat vertices[] =
{
	-0.5f, -0.5f, 0.0f, //left
	0.5f, -0.5f, 0.0f,  //right
	0.0f, 0.5f, 0.0f    //top
};

GLuint mainProgram, VAO, VBO;

void draw()
{
	glClearColor(0.2f, 0.9f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(mainProgram);

	glm::mat4 m = glm::mat4(1.0);
	glm::mat4 v = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 p = glm::perspective(90.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 mvp = p * v * m;
	
	GLint mvLoc = glGetUniformLocation(mainProgram, "modelView");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(v * m));
	GLint pLoc = glGetUniformLocation(mainProgram, "projection");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(p));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

int main()
{
	//Setup GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", nullptr, nullptr);

	//Ensure window was created
	if (window == nullptr)
	{
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
	if (glewInit() != GLEW_OK)
	{
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

	if (!success)
	{
		glGetProgramInfoLog(mainProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	delete(vShader);
	delete(fShader);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		//Check for events like key pressed, mouse moves, etc.
		glfwPollEvents();

		//draw
		draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}