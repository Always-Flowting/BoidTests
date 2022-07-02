#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <stdexcept>

#include "Display.h"

Display* pDisplay{ nullptr };

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int modifier);

int main()
{
	try
	{
		pDisplay = new Display{ 1200, 675, key_callback, mouse_callback };
	}
	catch (const std::exception& exception)
	{
		std::cerr << "ERROR::(" << exception.what() << ")\n";
		return -1;
	}

	pDisplay->init(50);

	while (!pDisplay->shouldClose())
	{
		pDisplay->clear(0.1f, 0.12f, 0.105f, 1.0f);

		pDisplay->draw();

		pDisplay->update(true, true);
	}

	delete pDisplay;
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	pDisplay->processKeyInput(key, scancode, action, mode);
}

void mouse_callback(GLFWwindow* window, int button, int action, int modifier)
{
	pDisplay->processMouseInput(button, action, modifier);
}