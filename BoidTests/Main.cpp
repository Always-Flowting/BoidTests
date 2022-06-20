#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <stdexcept>

#include "Display.h"

Display* display{ nullptr };

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int modifier);

int main()
{
	try
	{
		display = new Display{ 800, 450, key_callback, mouse_callback };
	}
	catch (const std::exception& exception)
	{
		std::cerr << "ERROR::(" << exception.what() << ")\n";
		return -1;
	}

	display->init();

	glLineWidth(2.0f);

	while (!display->shouldClose())
	{
		display->clear(0.1f, 0.1f, 0.25f, 1.0f);

		display->draw();

		display->update(true, true);
	}

	delete display;
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void mouse_callback(GLFWwindow* window, int button, int action, int modifier)
{
}