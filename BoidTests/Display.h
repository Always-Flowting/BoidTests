#ifndef BOID_DISPLAY_H
#define BOID_DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <exception>
#include <stdexcept>
#include <memory>

#include "ResourceManager.h"
#include "Flock.h"

class Display
{
private:
	GLFWwindow* m_window{};
	int m_width;
	int m_height;

	bool m_isClosed{ false };

	GLuint m_VAO{};
	GLuint m_VBO{};
	std::string m_flockShader{ "flock" };

	GLuint s_VAO{};
	GLuint s_VBO{};
	std::string m_slidShader{ "slider" };

	std::unique_ptr<Flock> m_flock{ nullptr };



	void updateObjects();

public:
	Display(int width, int height, GLFWkeyfun keycallback = nullptr, GLFWmousebuttonfun mousecallback = nullptr);
	~Display();

	void init();

	void clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void draw();
	void update(bool draw, bool pollevents);

	bool shouldClose() const { return m_isClosed; }
};

#endif