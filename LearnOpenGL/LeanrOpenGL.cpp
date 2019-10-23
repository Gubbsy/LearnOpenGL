// GLAD manages function pointers for OpenGL
#include <glad/glad.h>
// GLFW is an OpenGL window, event and context manager for openGL
#include <GLFW/glfw3.h>
#include <iostream>


using namespace std;

// Prototype function for later use (in code)
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Prototype function for input
void processInput(GLFWwindow* window);

//Shaders
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main()
{
	// Set-up glfw to provide conntext within openGL
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Preparing window for openGL (kill if window wasn't created)
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	//Set OpenGL context to window
	glfwMakeContextCurrent(window);

	//Initialising glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//tell Open GL window ize
	glViewport(0, 0, 800, 600);

	//-----------REGISTER GLFW CALLBACKS-------------//
	// Note: callbacks should be registered before   //
	// before the intial gameloop.					 //
	// Callbacks can be used to add custom proccess  //
	// for input and error handling etc.			 //
	//-----------------------------------------------//
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	////----------------------------------- Shader Set-up --------------------------------------//
	
	//VERTEX SHADER

	// Setting up vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Loading shader source into generated vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//compiling vertexshader
	glCompileShader(vertexShader);

	// Check if compilation of shader was correct
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	//FRAGMENT SHADER

	// Setting up fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Loading shader source into generated fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// compiling fragmentshader
	glCompileShader(fragmentShader);

	// Check if compilation of shader was correct
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// CREATING SHADER PROGRAM (PIPELINE)
	
	// Create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//Attach and link shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check linking worked
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER_PROGRAM::LINK::FAILED\n" << infoLog << endl;
	}
	

	//Clean-up shader resources once loaded in 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ------------------------------------- Vertices Set-up -----------------------------------//

	// Vertices to render
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
		};
	
	//VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//Bind it as VAO
	glBindVertexArray(VAO);

	//VBO
	//Create a Vertex Buffer Object and assigns it to the VBO integer pointer
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// Binds our generatd buffer to the GL_ARRAY_BUFFER state. (Tells the state machine to use our VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Taking our array of vertices and passing them into the bound VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// (How we pass the VBO into the shader) Specifying address within shader to send verticies to (location), colections size (vector 3), data type, does it need normaising, stride between data collection, initial offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// Enabling the ability location 0 
	glEnableVertexAttribArray(0);



	//----------------------------------------------------------------------------------------------//
	//Game loop
	while (!glfwWindowShouldClose(window))
	{
		//Input
		processInput(window);

		// Rendering comands here //

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Listens for events and calls defined call back && swap buffers.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//GLFW Resource Clean Up
	glfwTerminate();
	return 0;

}

// Call back function to resize viewport if user re-szes window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}