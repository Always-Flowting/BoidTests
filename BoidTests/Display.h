#ifndef BOID_DISPLAY_H
#define BOID_DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <exception>
#include <stdexcept>
#include <memory>
#include <vector>

#include "ResourceManager.h"
#include "Flock.h"
#include "Slider.h"

class Display
{
private:
	enum class objectState
	{
		hidden,
		frozen,
		normal,

		max_states
	};

	GLFWwindow* m_window{};
	int m_width;
	int m_height;

	bool m_isClosed{ false };

	objectState m_flockState{ objectState::normal };

	std::unique_ptr<Flock> m_flock{ nullptr };

	Slider m_slider{ glm::vec2{600.0f, 337.5f}, 1000.0f, 500.0f };




	void updateObjects();


public:
	Display(int width, int height, GLFWkeyfun keycallback = nullptr, GLFWmousebuttonfun mousecallback = nullptr);
	~Display();

	void init();
	void addFlockGroup(int amount, Boid::Type type, const glm::vec3& colour, const Boid::BoidVariables& variables);

	void clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void draw();
	void update(bool draw, bool pollevents);

	void processKeyInput(int key, int scancode, int action, int mode);
	void processMouseInput(int button, int action, int modifier);

	bool shouldClose() const { return m_isClosed; }
};

#endif