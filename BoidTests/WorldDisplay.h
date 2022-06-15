#ifndef WORLD_DIPLAY_H
#define WORLD_DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ResourceManager.h"
#include "Flock.h"

class DisplayWorld
{
private:
	GLFWwindow* m_window;
	int m_width{};
	int m_height{};


	GLuint m_VAO{};
	GLuint m_VBO{};
	std::string m_flockName{ "flock" };

	Flock m_flock;


public:
	DisplayWorld(GLFWwindow* window);
	~DisplayWorld();

	void init();

	void update();

	void render();
};

#endif