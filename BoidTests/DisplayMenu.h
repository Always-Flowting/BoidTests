#ifndef DISPLAY_MENU_H
#define DISPLAY_MENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class DisplayMenu
{
private:
	GLFWwindow* m_window;

	float* m_data;

public:
	DisplayMenu(GLFWwindow* window);
};

#endif
