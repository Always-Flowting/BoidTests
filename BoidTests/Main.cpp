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
	Flock flock{ window };
	flock.init(100);


	ResourceManager::loadShader("boid.vert", "boid.frag", "wireBoid.geom", "boy");

	GLuint VAO, VBO;

	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, flock.getByteSize(), flock.getData(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);

	float aspect{ wWidth / static_cast<float>(wHeight) };
	glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
	ResourceManager::getShader("boy").setMat4("proj", projection);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		if (flock.update())
		{
			ResourceManager::getShader("boy").activate();
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, flock.getByteSize(), flock.getData());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ResourceManager::getShader("boy").activate();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, flock.getAmount());
		glBindVertexArray(0);

		// swap the buffers so we can draw to a hidden one
		glfwSwapBuffers(window);
		// make sure glfw knows to look for anything that​ happens
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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