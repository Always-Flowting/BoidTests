#include "Flock.h"

Flock::Flock(GLFWwindow* window)
	:m_window{ window }
{
	glfwGetWindowSize(m_window, &m_width, &m_height);
}

Flock::~Flock()
{
	delete[] m_data;
}

void Flock::init(int preyNum, int predNum)
{
	m_preyAmount = preyNum;
	m_predAmount = predNum;
	m_flock.clear();
	m_flock.resize(m_predAmount + m_preyAmount);
	m_data = new float[4 * (m_predAmount + m_preyAmount)];

	for (int i{ 0 }; i < (m_predAmount + m_preyAmount); ++i)
	{
		if (i < m_preyAmount)
		{
			m_flock[i] = Boid{ glm::vec2(m_width / 2.0f, m_height / 2.0f), 0.03f, 4.0f, 50.0f, 25.0f, 4.0f, Boid::Type::prey };
		}
		else
		{
			m_flock[i] = Boid{ glm::vec2(m_width / 2.0f, m_height / 2.0f), 0.03f, 4.0f, 50.0f, 25.0f, 8.0f, Boid::Type::prey };
		}
	}

	updateData();
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

void Flock::seek(Boid& boid, float weight)
{
	double xPos, yPos;
	glfwGetCursorPos(m_window, &xPos, &yPos);
	glm::vec2 target{ static_cast<float>(xPos), m_height - static_cast<float>(yPos) };
	glm::vec2 steering = target - boid.getPosition();
	steering = glm::normalize(steering) * boid.getMaxVelocity();

	steering -= boid.getVelocity();

	boid.addAcceleration(weight * steering);
}

void Flock::border(Boid& boid)
{
	if (boid.getPosition().x < -boid.getSize())
	{
		boid.setPosition(glm::vec2(static_cast<float>(m_width) + boid.getSize(), boid.getPosition().y));
	}
	if (boid.getPosition().x > static_cast<float>(m_width) + boid.getSize())
	{
		boid.setPosition(glm::vec2(-boid.getSize(), boid.getPosition().y));
	}
	if (boid.getPosition().y < -boid.getSize())
	{
		boid.setPosition(glm::vec2(boid.getPosition().x, static_cast<float>(m_height) + boid.getSize()));
	}
	if (boid.getPosition().y > static_cast<float>(m_height) + boid.getSize())
	{
		boid.setPosition(glm::vec2(boid.getPosition().x, -boid.getSize()));
	}
}

bool Flock::update()
{
	if (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_lastUpdate).count() >= m_updatePeriod)
	{
		if (m_currentMode == Behaviour::seek_and_flee)
		{
			for (auto& boid : m_flock)
			{
				seek(boid, 1.0f);
				border(boid);
			}
		}
		
		for (auto& boid : m_flock)
		{
			boid.update();
		}
		updateData();

		m_lastUpdate = std::chrono::high_resolution_clock::now();
		return true;
	}
	return false;
}