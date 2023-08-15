#include <iostream>

#include <glad/glad.h> // must be included before glfw3.h
#include <glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\0";

int main()
{
	printf("Hello World!\n");

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for Mac OS X

	// Create a window object
	GLFWwindow* window = glfwCreateWindow(
		SCR_WIDTH,			// width
		SCR_HEIGHT,			// height
		"LearnOpenGL",	// title
		NULL,			// monitor
		NULL			// share
	);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); // clean up all GLFW resources
		return -1;
	}

	// make the context of the specified window current on the calling thread
	glfwMakeContextCurrent(window); 

	// Initialize GLAD before calling any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set the viewport of OpenGL
	glViewport(
		0, // lower left corner of the window
		0,  // lower left corner of the window
		SCR_WIDTH, // width
		SCR_HEIGHT // height
	);

	// register a callback function on window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// create a vertex shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(
		GL_VERTEX_SHADER // shader type
	);
	glShaderSource(
		vertexShader,			// shader
		1,						// the number of strings
		&vertexShaderSource,	// an array of pointers to strings containing the source code to be loaded into the shader
		NULL					// an array of string lengths
	); // sets the source code in shader to the source code in the array of strings specified by string
	glCompileShader(vertexShader); // compiles the source code strings that have been stored in the shader object specified by shader

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(
		vertexShader,		// shader
		GL_COMPILE_STATUS,	// pname
		&success			// params
	); // returns a parameter from a shader object
	if (!success)
	{
		glGetShaderInfoLog(
			vertexShader,	// shader
			512,			// bufSize
			NULL,			// length
			infoLog			// infoLog
		); // returns the information log for a shader object
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// create a fragment shader object
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // check for linking errors
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); // get the linking errors
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// activate the shader program
	glUseProgram(shaderProgram);

	// delete the shader objects once they've been linked into the program object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*
		NDC (Normalized Device Coordinates)
		- OpenGL 은 모든 좌표를 -1.0 ~ 1.0 사이로 정규화한다. 이러한 정규화된 좌표를 NDC 라고 한다.
		- 정규화된 좌표를 벗어난 좌표는 버려진다. (삭제, 클리핑)
		- 정규화된 좌표를 사용하면, 어떤 해상도의 모니터에서든 동일한 결과를 얻을 수 있다.
		- NDC 는 최종적으로 screen coordinates 로 변환된다. 이러한 변환을 viewport transformation 이라고 한다.
	*/
	// defines for the vertex shader
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// create a vertex array object
	unsigned int VAO;
	glGenVertexArrays(
		1,		// n
		&VAO	// arrays
	); // generates vertex array object names
	glBindVertexArray(VAO); // binds a vertex array object

	// create a vertex buffer object (VBO)
	unsigned int VBO; // vertex buffer object
	glGenBuffers(
		1,		// the number of buffer objects to be generated
		&VBO	// the address of an unsigned int variable to store the name of the first buffer object generated
	); // generate a buffer object name
	glBindBuffer(
		GL_ARRAY_BUFFER, // target
		VBO // buffer
	); // bind a named buffer object
	glBufferData(
		GL_ARRAY_BUFFER,	// target, current context 의 array buffer 에 바인딩된 버퍼를 선택
		sizeof(vertices),	// size
		vertices,			// data
		GL_STATIC_DRAW		// usage, 데이터는 한 번만 설정되고, 그 후에는 읽히기만 한다.
	); // creates and initializes a buffer object's data store

	// create an element buffer object (EBO)
	unsigned int EBO; // element buffer object
	glGenBuffers(1, &EBO); // generate a buffer object name
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind a named buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // creates and initializes a buffer object's data store


	// link vertex attributes
	glVertexAttribPointer(
		0,			// index - the location of the vertex attribute
		3,			// size
		GL_FLOAT,	// type - the data type of each component in the array
		GL_FALSE,	// normalized - whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed
		3 * sizeof(float), // stride
		(void*)0	// pointer
	); // specifies the location and data format of the array of generic vertex attributes at index index to use when rendering
	glEnableVertexAttribArray(0); // enables a generic vertex attribute array

	//	// 0. copy our vertices array in a buffer for OpenGL to use
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the Vertex Buffer Object first before calling glVertexAttribPointer
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copies the previously defined vertex data into the buffer's memory
	//	// 1. then set and enable vertex attributes pointers
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // specifies the location and data format of the array of generic vertex attributes at index index to use when rendering
	//	glEnableVertexAttribArray(0); // enables a generic vertex attribute array
	//	// 2. use our shader program when we want to render an object
	//	glUseProgram(shaderProgram); // tells OpenGL to use the shader program we've compiled
	//	// 3. now draw the object
	//	someOpenglFunctionThatDrawsOurTriangle(); // draws the object


	//	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	//	// 1. bind Vertex Array Object
	//	glBindVertexArray(VAO);
	//	// 2. copy our vertices array in a buffer for OpenGL to use
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//	// 3. then set our vertex attributes pointers
	//	glVertexAttribPointer(0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);
	//	// ..:: Drawing code (in render loop) :: ..
	//	// 4. draw the object
	//	glUseProgram(shaderProgram);
	//	glBindVertexArray(VAO);
	//	someOpenglFunctionThatDrawsOurTriangle(); // draws the object

	//	// ..:: Initialization code :: ..
	//	// 1. bind Vertex Array Object
	//	glBindVertexArray(VAO);
	//	// 2. copy our vertices array in a buffer for OpenGL to use
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//	// 3. copy our index array in a element buffer for OpenGL to use
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//	// 4. then set the vertex attributes pointers
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);
	//	// ..:: Drawing code (in render loop) :: ..
	//	glUseProgram(shaderProgram);
	//	glBindVertexArray(VAO);
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//	glBindVertexArray(0);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window); // check if the user has pressed the escape key

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function, Clear 색상 지정
		glClear(GL_COLOR_BUFFER_BIT); // state-using function, 지정된 Clear 색으로 특정 Buffer 를 Clear

		//glPolygonMode(
		//	GL_FRONT_AND_BACK,	// face
		//	GL_LINE				// mode
		//); // sets the polygon rasterization mode of the active polygon primitive
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // default

		// be sure to activate the shader
		glUseProgram(shaderProgram);

		// update shader uniform
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // -1.0 ~ 1.0
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // returns the location of the uniform variable for the current program object
		glUniform4f(
			vertexColorLocation, // location
			0.0f, // value, redValue
			greenValue, // value, greenValue
			0.0f, // value, blueValue
			1.0f // value, alphaValue
		); // specifies the value of a uniform variable for the current program object

		glBindVertexArray(VAO);
		glDrawElements(
			GL_TRIANGLES,		// mode
			6,					// count, the number of elements to be rendered
			GL_UNSIGNED_INT,	// type
			0					// indices, element array buffer offset
		); // draws primitives from array data
		glBindVertexArray(0); // unbind the VAO

		// check and call events and swap the buffers
		glfwSwapBuffers(window); // swap the color buffer
		glfwPollEvents(); // check if any events are triggered
	}

	glBindVertexArray(0); // unbind the VAO
	glBindBuffer(GL_VERTEX_ARRAY, 0); // unbind the VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the EBO

	glDeleteVertexArrays(1, &VAO); // delete the VAO
	glDeleteBuffers(1, &VBO); // delete the VBO
	glDeleteBuffers(1, &EBO); // delete the EBO
	glDeleteProgram(shaderProgram); // delete the shader program

	glfwTerminate(); // clean up all GLFW resources
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// close the window
		glfwSetWindowShouldClose(window, true);
	}
}
