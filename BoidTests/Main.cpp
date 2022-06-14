#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Flock.h"

int wWidth{ 1600 };
int wHeight{ 900 };

// take input while on the window and do something with it
void processInput(GLFWwindow* window);

int main()
{
	// initilize and set up glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window object
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window{ glfwCreateWindow(wWidth, wHeight, "window", nullptr, nullptr) };

	// error handling for if window can not set up correctly
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// make the window what glfw is outputtin to
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	// flock setup
	Flock flock{window, wWidth, wHeight };
	flock.init(100, 10);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		flock.update();
		flock.processInput();
		
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		flock.render();

		// swap the buffers so we can draw to a hidden one
		glfwSwapBuffers(window);
		// make sure glfw knows to look for anything that​ happens
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// take input while on the window and do something with it
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}