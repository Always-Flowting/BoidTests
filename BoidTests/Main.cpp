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

	pDisplay->init();

	pDisplay->addFlockGroup(250, Boid::Type::prey, glm::vec3(0.02f, 0.87f, 0.34f), 
		Boid::BoidVariables{ 0.06f, 4.0f, 25.0f, 90.0f, 3.0f, 1.2f, 1.0f, 0.667f });

	//display->addFlockGroup(75, Boid::Type::prey, glm::vec3(0.7f, 0.87f, 0.16f),
	//	Boid::BoidVariables{ 0.03f, 2.0f, 15.0f, 100.0f, 2.0f, 10.6f, 04.3f, 10.5f });

	pDisplay->addFlockGroup(10, Boid::Type::predator, glm::vec3(1.0f, 0.15f, 0.02f), 
		Boid::BoidVariables{ 0.06f, 2.0f, 55.0f, 120.0f, 4.5f, 1.5f, 0.667f, 0.667f });

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