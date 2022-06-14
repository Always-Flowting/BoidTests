#ifndef WORLD_DIPLAY_H
#define WORLD_DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class DisplayWorld
{
private:
	GLFWwindow* m_window;

public:
	DisplayWorld(GLFWwindow* window);

	void init();
};

#endif