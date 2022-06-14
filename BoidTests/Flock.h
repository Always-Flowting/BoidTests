#ifndef FLOCK_CLASS_H
#define FLOCK_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <chrono>

#include "Boid.h"
#include "ResourceManager.h"

class Flock
{
private:
	enum class Behaviour
	{
		seek_and_flee,
		pursue_and_evade,
		flock,

		max_behaviours
	};

	std::vector<Boid> m_flock{};
	float* m_data{ nullptr };
	GLFWwindow* m_window;
	int m_width;
	int m_height;

	int m_preyAmount{};
	int m_predAmount{};

	GLuint m_VAO{};
	GLuint m_VBO{};
	std::string m_shaderName{ "boids" };

	double m_frameRate{ 1.0 / 60.0 };
	std::chrono::high_resolution_clock::time_point m_lastFrame{ std::chrono::high_resolution_clock::now() };

	glm::vec2 m_lastMouse{};
	Behaviour m_currentMode{ Behaviour::seek_and_flee };
	bool m_clicking{ false };

	void updateData();

public:
	Flock(GLFWwindow* window, int width, int height);
	~Flock();

	void init(int preyNum, int predNum = 0);
	void processInput();
	void update();
	void render();

	std::size_t getByteSize() const { return 4 * (m_preyAmount + m_predAmount) * sizeof(float); }
	int getAmount() const { return (m_preyAmount + m_predAmount); }
	float* getData() { return m_data; }
};

#endif

