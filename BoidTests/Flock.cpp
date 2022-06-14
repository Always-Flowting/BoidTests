#include "Flock.h"

Flock::Flock(GLFWwindow* window, int width, int height)
	:m_width{ width },
	m_height{ height },
	m_window{ window }
{ }

Flock::~Flock()
{
	delete[] m_data;
}

void Flock::init(int preyNum, int predNum)
{
	m_preyAmount = preyNum;
	m_predAmount = predNum;
	m_flock.resize(m_predAmount + m_preyAmount);
	m_data = new float[4 * (m_predAmount + m_preyAmount)];
	for (int i{ 0 }; i < (m_predAmount + m_preyAmount); ++i)
	{
		if (i < m_preyAmount)
		{
			m_flock[i] = Boid{ glm::vec2(m_width / 2.0f, m_height / 2.0f), Boid::Type::prey, 4.0f };
		}
		else
		{
			m_flock[i] = Boid{ glm::vec2(m_width / 2.0f, m_height / 2.0f), Boid::Type::predator, 8.0f };
		}
	}

	Boid::setBorder(m_width, m_height);
	Boid::setFlock(&m_flock);

	updateData();


	ResourceManager::loadShader("boid.vert", "boid.frag", "wireBoid.geom", m_shaderName);

	glGenBuffers(1, &m_VBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, getByteSize(), m_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);

	float aspect{ m_width / static_cast<float>(m_height) };
	glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
	ResourceManager::getShader("boids").setMat4("proj", projection);
}

void Flock::processInput()
{
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double xPos, yPos;
		glfwGetCursorPos(m_window, &xPos, &yPos);
		m_lastMouse = glm::vec2{ static_cast<float>(xPos), static_cast<float>(m_height) - static_cast<float>(yPos) };
		m_clicking = true;
	}
	else
	{
		m_clicking = false;
	}

	if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_currentMode = Behaviour::seek_and_flee;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_currentMode = Behaviour::pursue_and_evade;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS)
	{
		m_currentMode = Behaviour::flock;
	}
}

void Flock::updateData()
{
	float aspect{ m_width / static_cast<float>(m_height) };
	for (int i{ 0 }; i < (m_preyAmount + m_predAmount); ++i)
	{
		m_data[(4 * i)] = (2.0f * aspect * m_flock[i].getPosition().x / m_width) - aspect;
		m_data[(4 * i) + 1] = (2.0f * m_flock[i].getPosition().y / m_height) - 1.0f;
		m_data[(4 * i) + 2] = m_flock[i].getAngle();
		m_data[(4 * i) + 3] = (2.0f * m_flock[i].getSize() / m_height);
	}
}

void Flock::update()
{
	if (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() 
		- m_lastFrame).count() >= m_frameRate)
	{
		if (m_clicking)
		{
			for (auto& boid : m_flock)
			{
				if (boid.getType() == Boid::Type::prey)
				{
					boid.flee(m_lastMouse);
				}
				else
				{
					boid.seek(m_lastMouse);
				}
			}
		}
		else if (m_currentMode == Behaviour::seek_and_flee)
		{
			for (auto& boid : m_flock)
			{
				if (boid.getType() == Boid::Type::prey)
				{
					boid.flee( Boid::Type::predator);
				}
				else
				{
					boid.seek( Boid::Type::prey );
				}
			}
		}
		else if (m_currentMode == Behaviour::pursue_and_evade)
		{
			for (auto& boid : m_flock)
			{
				if (boid.getType() == Boid::Type::prey)
				{
					boid.evade(Boid::Type::predator);
				}
				else
				{
					boid.pursue(Boid::Type::prey);
				}
			}
		}
		else if (m_currentMode == Behaviour::flock)
		{
			for (auto& boid : m_flock)
			{
				boid.seperate(1.5f);
				boid.align(1.0f);
				boid.cohesion(1.0f);
			}
		}
		
		for (auto& boid : m_flock)
		{
			boid.update();
		}
		updateData();

		ResourceManager::getShader(m_shaderName).activate();
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, getByteSize(), m_data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_lastFrame = std::chrono::high_resolution_clock::now();
	}
}

void Flock::render()
{
	ResourceManager::getShader(m_shaderName).activate();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, (m_preyAmount + m_predAmount));
	glBindVertexArray(0);
}