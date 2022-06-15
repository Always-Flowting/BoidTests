#include "WorldDisplay.h"

DisplayWorld::DisplayWorld(GLFWwindow* window)
	:m_window{ window },
	m_flock{ window }
{
	glfwGetWindowSize(m_window, &m_width, &m_height);
}

DisplayWorld::~DisplayWorld()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void DisplayWorld::init()
{
	m_flock.init(250, 50);

	ResourceManager::loadShader("boid.vert", "boid.frag", "wireBoid.geom", m_flockName);

	glGenBuffers(1, &m_VBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_flock.getByteSize(), m_flock.getData(), GL_STATIC_DRAW);

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
	ResourceManager::getShader(m_flockName).setMat4("proj", projection);
}

void DisplayWorld::update()
{
	if (m_flock.run())
	{
		ResourceManager::getShader(m_flockName).activate();
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_flock.getByteSize(), m_flock.getData());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void DisplayWorld::render()
{
	ResourceManager::getShader(m_flockName).activate();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, m_flock.getAmount());
	glBindVertexArray(0);
}