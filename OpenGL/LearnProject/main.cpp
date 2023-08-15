#include <iostream>

#include <glad/glad.h> // must be included before glfw3.h
#include <glfw3.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// close the window
		glfwSetWindowShouldClose(window, true);
	}
};

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
		800,			// width
		600,			// height
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
		800, // width
		600 // height
	);

	// register a callback function on window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window); // check if the user has pressed the escape key

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function, Clear 색상 지정
		glClear(GL_COLOR_BUFFER_BIT); // state-using function, 지정된 Clear 색으로 특정 Buffer 를 Clear


		// check and call events and swap the buffers
		glfwSwapBuffers(window); // swap the color buffer
		glfwPollEvents(); // check if any events are triggered
	}

	glfwTerminate(); // clean up all GLFW resources
	return 0;
}
