#include <iostream>

#include <glad/glad.h> // must be included before glfw3.h
#include <glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	ourColor = aColor;\n"
"}\0";
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
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

	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");
	//Shader ourShader("shaders/shader.vs", "shaders/_1_6_shader_sol1.fs");

	/*
		NDC (Normalized Device Coordinates)
		- OpenGL 은 모든 좌표를 -1.0 ~ 1.0 사이로 정규화한다. 이러한 정규화된 좌표를 NDC 라고 한다.
		- 정규화된 좌표를 벗어난 좌표는 버려진다. (삭제, 클리핑)
		- 정규화된 좌표를 사용하면, 어떤 해상도의 모니터에서든 동일한 결과를 얻을 수 있다.
		- NDC 는 최종적으로 screen coordinates 로 변환된다. 이러한 변환을 viewport transformation 이라고 한다.
	*/
	// defines for the vertex shader
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,	// first triangle
		1, 2, 3		// second triangle
	};
	float texCoords[] = {
		0.0f, 0.0f, // lower-left corner
		1.0f, 0.0f, // lower-right corner
		0.5f, 1.0f  // top-center corner
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
		8 * sizeof(float), // stride
		(void*)0	// offset
	); // specifies the location and data format of the array of generic vertex attributes at index index to use when rendering
	glEnableVertexAttribArray(0); // enables a generic vertex attribute array
	glVertexAttribPointer(
		1, // index, the location of the vertex attribute
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		8 * sizeof(float), // stride, (vec3 position, vec3 color) => need to skip 6 floats
		(void*)(3 * sizeof(float)) // offset, (vec3 position, vec3 color) => need to skip 3 floats (12 bytes)
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		2, // index, the location of the vertex attribute
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		8 * sizeof(float), // stride, (vec3 position, vec3 color, vec2 texCoords) => need to skip 5 floats
		(void*)(6 * sizeof(float)) // offset, (vec3 position, vec3 color, vec2 texCoords) => need to skip 3 floats (12 bytes)
	);
	glEnableVertexAttribArray(2);


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

	// load and create a texture
	// -------------------------

	unsigned int texture;
	glGenTextures(1, &texture); // generates texture names

	glBindTexture(GL_TEXTURE_2D, texture); // bind a named texture to a texturing target

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // flips the loaded image vertically since OpenGL expects the 0.0 coordinate on the y-axis to be on the bottom side of the image
	unsigned char* data = stbi_load("resources/container.jpg", // path
		&width, // width
		&height, // height
		&nrChannels, // number of channels
		0 // desired channels
	); // loads an image from the file system into a buffer

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, // target
			0, // level
			GL_RGB, // internal format
			width, // width
			height, // height
			0, // border
			GL_RGB, // format
			GL_UNSIGNED_BYTE, // type
			data // data
		); // creates a texture image
		glGenerateMipmap(GL_TEXTURE_2D); // generates mipmaps for the currently bound texture object
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data); // frees image memory

	unsigned int texture2;
	glGenTextures(1, &texture2); // generates texture names
	glBindTexture(GL_TEXTURE_2D, texture2); // bind a named texture to a texturing target
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unsigned char* data2 = stbi_load("resources/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2); // frees image memory

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

		glActiveTexture(GL_TEXTURE0); // activates the texture unit
		glBindTexture(GL_TEXTURE_2D, texture); // bind a named texture to a texturing target
		glActiveTexture(GL_TEXTURE1); // activates the texture unit
		glBindTexture(GL_TEXTURE_2D, texture2); // bind a named texture to a texturing target

		// be sure to activate the shader
		ourShader.use(); // glUseProgram(shaderProgram);

		// update shader uniform
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // -1.0 ~ 1.0
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // returns the location of the uniform variable for the current program object
		//glUniform4f(
		//	vertexColorLocation, // location
		//	0.0f, // value, redValue
		//	greenValue, // value, greenValue
		//	0.0f, // value, blueValue
		//	1.0f // value, alphaValue
		//); // specifies the value of a uniform variable for the current program object

		glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // set it manually
		ourShader.setInt("texture2", 1); // or with shader class

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
