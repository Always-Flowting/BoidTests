#include "Display.h"

Display::Display(int width, int height, GLFWkeyfun keycallback, GLFWmousebuttonfun mousecallback)
	:m_width{ width },
	m_height{ height }
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window object
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(m_width, m_height, "BoidSimulation", nullptr, nullptr);

	// error handling for if window can not set up correctly
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error{ "Failed to create GLFW window" };
	}
	// make the window what glfw is outputtin to
	glfwMakeContextCurrent(m_window);

	glfwSetKeyCallback(m_window, keycallback);
	glfwSetMouseButtonCallback(m_window, mousecallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error{ "Failed to initialize GLAD" };
	}

	m_flock = std::unique_ptr<Flock>{ new Flock{m_width, m_height} };
	m_menu = std::unique_ptr<Menu>{ new Menu{m_width, m_height} };
}

Display::~Display()
{
	glfwTerminate();
}

void Display::addFlockGroup(int amount, Boid::Type type, const glm::vec3& colour, const Boid::BoidVariables& variables)
{
	m_flock->addGroup(amount, type, colour, variables);

	
}

void Display::updateObjects()
{
	if (m_flockState == objectState::normal)
	{
		m_flock->run();
	}
	if (m_menu->sliderActive())
	{
		double xPos, yPos;
		glfwGetCursorPos(m_window, &xPos, &yPos);
		glm::vec2 pos{ static_cast<float>(xPos), static_cast<float>(m_height - yPos) };
		m_menu->MoveActiveSlider(pos.x);
	}
}

void Display::clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Display::draw()
{
	if (m_flockState >= objectState::frozen)
	{
		m_flock->render();
	}
	if (m_showMenu)
	{
		m_menu->render();
	}
}

void Display::update(bool draw, bool pollevents)
{
	//  Update Display
	if (draw) {
		glfwSwapBuffers(m_window);
	}

	//  Check for key press etc.
	if (pollevents) {
		glfwPollEvents();
	}

	//  Exit
	if (glfwWindowShouldClose(m_window)) {
		m_isClosed = true;
	}

	updateObjects();
}

void Display::processKeyInput(int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (m_flockState == objectState::frozen)
		{
			m_flockState = objectState::normal;
		}
		else
		{
			m_flockState = objectState::frozen;
		}
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		m_showMenu = !m_showMenu;
	}
}

void Display::processMouseInput(int button, int action, int modifier)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && m_showMenu)
	{
		double xPos, yPos;
		glfwGetCursorPos(m_window, &xPos, &yPos);
		glm::vec2 pos{ static_cast<float>(xPos), static_cast<float>(m_height - yPos) };
		m_menu->setActiveSlider(pos);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		m_menu->resetActiveSlider();
	}
}