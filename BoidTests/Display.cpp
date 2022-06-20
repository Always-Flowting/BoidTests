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
}

Display::~Display()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);

	glDeleteVertexArrays(1, &s_VAO);
	glDeleteBuffers(1, &s_VBO);
	glfwTerminate();
}

void Display::init()
{
	m_flock = std::unique_ptr<Flock>{ new Flock{m_window} };
	m_flock->init(30, 30, 30, 10, 10, 10);
	m_flock->setVariables(Boid::Type::prey1, 1.0f, 0.66666f, 0.66666f);
	m_flock->setVariables(Boid::Type::prey2, 1.0f, 0.66666f, 0.66666f);
	m_flock->setVariables(Boid::Type::prey3, 1.0f, 0.66666f, 0.66666f);
	m_flock->setVariables(Boid::Type::predator1, 2.0f, 0.5f, 1.5f);
	m_flock->setVariables(Boid::Type::predator2, 2.0f, 0.5f, 1.5f);
	m_flock->setVariables(Boid::Type::predator3, 2.0f, 0.5f, 1.5f);

	ResourceManager::loadShader("boid.vert", "boid.frag", "wireBoid.geom", m_flockShader);

	glGenBuffers(1, &m_VBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_flock->getByteSize(), m_flock->getData(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);

	float aspect{ m_width / static_cast<float>(m_height) };
	glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);

	ResourceManager::getShader(m_flockShader).setMat4("proj", projection);
}

void Display::updateObjects()
{
	if (m_flock->run())
	{
		ResourceManager::getShader(m_flockShader).activate();
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_flock->getByteSize(), m_flock->getData());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Display::clear(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Display::draw()
{
	ResourceManager::getShader(m_flockShader).activate();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, m_flock->getAmount());
	glBindVertexArray(0);
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