#include "Flock.h"

/*
/ All seeking based function
*/

// seek the position specified
void Flock::seekPosition(Boid& boid, float weight, const glm::vec2& targetPosition)
{
	glm::vec2 steering = targetPosition - boid.getPosition();
	steering = glm::normalize(steering) * boid.getMaxVelocity();

	steering -= boid.getVelocity();

	boid.addAcceleration(weight * steering);
}

// seek the closest boid
void Flock::seekClosest(Boid& boid, float weight)
{
	glm::vec2 target{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
		if (dist < glm::length(target) && dist < boid.getSenseDistance() && dist > 0.0f)
		{
			target = other.getPosition();
			found = true;
		}
		else if (!found && dist < other.getSenseDistance() && dist > 0.0f)
		{
			target = other.getPosition();
			found = true;
		}
	}

	if (found)
	{
		seekPosition(boid, weight, target);
	}
}

// seek the closest speficied type of boid
void Flock::seekClosestType(Boid& boid, float weight, Boid::Type type)
{
	glm::vec2 target{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		if (other.getType() == type)
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition())};
			if (dist < glm::length(target) && dist < boid.getSenseDistance() && dist > 0.0f)
			{
				target = other.getPosition();
				found = true;
			}
			else if (!found && dist < other.getSenseDistance() && dist > 0.0f)
			{
				target = other.getPosition();
				found = true;
			}
		}
	}

	if (found)
	{
		seekPosition(boid, weight, target);
	}
}

// seek if psoition specified is in range
void Flock::rangeSeek(Boid& boid, float weight, const glm::vec2& targetPosition)
{
	float dist{ glm::distance(boid.getPosition(), targetPosition) };
	if (dist <= boid.getSenseDistance())
	{
		seekPosition(boid, weight, targetPosition);
	}
}

/*
/ All fleeing based functions
*/

// flee from a set position
void Flock::fleePosition(Boid& boid, float weight, const glm::vec2& targetPosition)
{
	glm::vec2 steering = boid.getPosition() - targetPosition;
	steering = glm::normalize(steering) * boid.getMaxVelocity();

	steering -= boid.getVelocity();

	boid.addAcceleration(weight * steering);
}

// flee from closest boid
void Flock::fleeClosest(Boid& boid, float weight)
{
	glm::vec2 target{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
		if (dist < glm::length(target) && dist < boid.getSenseDistance() && dist > 0.0f)
		{
			target = other.getPosition();
			found = true;
		}
		else if (!found && dist < other.getSenseDistance() && dist > 0.0f)
		{
			target = other.getPosition();
			found = true;
		}
	}

	if (found)
	{
		fleePosition(boid, weight, target);
	}
}

// flee from closest type of boid
void Flock::fleeClosestType(Boid& boid, float weight, Boid::Type type)
{
	glm::vec2 target{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		if (other.getType() == type)
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist < glm::length(target) && dist < boid.getSenseDistance() && dist > 0.0f)
			{
				target = other.getPosition();
				found = true;
			}
			else if (!found && dist < other.getSenseDistance() && dist > 0.0f)
			{
				target = other.getPosition();
				found = true;
			}
		}
	}

	if (found)
	{
		fleePosition(boid, weight, target);
	}
}

// flee if position specified is in range
void Flock::rangeFlee(Boid& boid, float weight, const glm::vec2& targetPosition)
{
	float dist{ glm::distance(boid.getPosition(), targetPosition) };
	if (dist <= boid.getSenseDistance())
	{
		fleePosition(boid, weight, targetPosition);
	}
}



void Flock::arrive(Boid& boid, float weight, const glm::vec2& target)
{
	glm::vec2 offset{ target - boid.getPosition() };
	float dist{ glm::length(offset) };
	if (dist > 0.0f)
	{
		float ramped_speed{ boid.getMaxVelocity() * dist / 500.0f};
		float clipped{ std::min(boid.getMaxVelocity(), ramped_speed) };
		glm::vec2 steering{ (clipped / dist) * offset };
		steering = steering - boid.getVelocity();

		boid.addAcceleration(steering);
	}

}

void Flock::seperate(Boid& boid, float weight)
{
	glm::vec2 steering{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		if (boid.getType() == other.getType())
		{

			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist > 0.0f && dist <= boid.getSeperationistance())
			{
				steering += glm::normalize(boid.getPosition() - other.getPosition()) / dist;
				found = true;
			}
		}
	}

	if (found)
	{
		steering = glm::normalize(steering) * boid.getMaxVelocity();
		steering -= boid.getVelocity();

		boid.addAcceleration(weight * steering);
	}
}

void Flock::align(Boid& boid, float weight)
{
	glm::vec2 direction{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		if (boid.getType() == other.getType())
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist > 0.0f && dist <= boid.getSenseDistance())
			{
				direction += other.getVelocity();
				found = true;
			}
		}
	}

	if (found)
	{
		direction = glm::normalize(direction) * boid.getMaxVelocity();
		direction -= boid.getVelocity();

		boid.addAcceleration(weight * direction);
	}
}

void Flock::cohesion(Boid& boid, float weight)
{
	glm::vec2 target{ 0.0f };
	int count{ 0 };

	for (auto& other : m_flock)
	{
		if (boid.getType() == other.getType())
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist > 0.0f && dist <= boid.getSenseDistance())
			{
				target += other.getPosition();
				++count;
			}
		}
	}

	if (count > 0)
	{
		target /= static_cast<float>(count);
		seekPosition(boid, weight, target);
	}
}

void Flock::eat(Boid& boid, float radius)
{
	if (boid.getType() == Boid::Type::predator)
	{
		static std::vector<int> toRemove{};
		for (int i{ 0 }; i < m_preyAmount; ++i)
		{
			float dist{ glm::distance(boid.getPosition(), m_flock[i].getPosition())};
			if (dist < radius)
			{
				toRemove.push_back(i);
			}
		}

		if (toRemove.size() > 0)
		{
			removeResize(toRemove);
		}
	}
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

void Flock::removeResize(std::vector<int>& toRemove)
{
	std::sort(toRemove.begin(), toRemove.end());
	for (auto it{toRemove.rbegin()}; it != toRemove.rend(); ++it)
	{
		m_flock.erase(m_flock.begin() + *it);
	}
	m_preyAmount -= toRemove.size();
	toRemove.clear();
	delete[] m_data;
	m_data = new float[7 * (m_preyAmount + m_predAmount)];
	updateData();
}

void Flock::updateData()
{
	float aspect{ m_width / static_cast<float>(m_height) };
	for (int i{ 0 }; i < (m_preyAmount + m_predAmount); ++i)
	{
		m_data[(7 * i)] = (2.0f * aspect * m_flock[i].getPosition().x / m_width) - aspect;
		m_data[(7 * i) + 1] = (2.0f * m_flock[i].getPosition().y / m_height) - 1.0f;
		m_data[(7 * i) + 2] = m_flock[i].getAngle();
		m_data[(7 * i) + 3] = (2.0f * m_flock[i].getSize() / m_height);
		m_data[(7 * i) + 4] = Boid::getColour(m_flock[i].getType()).r;
		m_data[(7 * i) + 5] = Boid::getColour(m_flock[i].getType()).g;
		m_data[(7 * i) + 6] = Boid::getColour(m_flock[i].getType()).b;
	}
}




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
	m_data = new float[7 * (m_predAmount + m_preyAmount)];

	for (int i{ 0 }; i < (m_predAmount + m_preyAmount); ++i)
	{
		if (i < m_preyAmount)
		{
			m_flock[i] = Boid{ glm::vec2(m_width / 2.0f, m_height / 2.0f), 0.06f, 6.0f, 100.0f, 45.0f, 6.0f, Boid::Type::prey };
		}
		else
		{
			m_flock[i] = Boid{ glm::vec2(2.0f, 2.0f), 0.09f, 9.0f, 150.0f, 125.0f, 8.0f, Boid::Type::predator };
		}
	}

	m_weights = {
		{Boid::Type::prey, {1.5f, 1.0f, 1.0f}},
		{Boid::Type::predator, {2.0f, 0.8f, 1.3f}}
	};

	updateData();
}

void Flock::processInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_0) == GLFW_PRESS)
	{
		m_currentMode = MouseAction::none;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_currentMode = MouseAction::all_seek;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_currentMode = MouseAction::all_flee;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_3) == GLFW_PRESS)
	{
		m_currentMode = MouseAction::seek_in_range;
	}
	else if (glfwGetKey(m_window, GLFW_KEY_4) == GLFW_PRESS)
	{
		m_currentMode = MouseAction::flee_in_range;
	}

	double xPos, yPos;
	glfwGetCursorPos(m_window, &xPos, &yPos);
	m_mousePosition.x = static_cast<float>(xPos);
	m_mousePosition.y = static_cast<float>(m_height) - static_cast<float>(yPos);
}

// update the acceleration of boid depending on current behaviour then update the velocity and position. return true if boids were upated
bool Flock::run()
{
	processInput();
	if (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_lastUpdate).count() >= m_updatePeriod)
	{
		for (auto& boid : m_flock)
		{
			seperate(boid, m_weights[Boid::Type::predator][0]);
			align(boid, m_weights[Boid::Type::predator][1]);
			cohesion(boid, m_weights[Boid::Type::predator][2]);
			eat(boid, 10.0f);
			if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				switch (m_currentMode)
				{
				case Flock::MouseAction::all_seek:
					seekPosition(boid, 5.0f, m_mousePosition);
					break;
				case Flock::MouseAction::all_flee:
					fleePosition(boid, 5.0f, m_mousePosition);
					break;
				case Flock::MouseAction::seek_in_range:
					rangeSeek(boid, 5.0f, m_mousePosition);
					break;
				case Flock::MouseAction::flee_in_range:
					rangeFlee(boid, 5.0f, m_mousePosition);
					break;
				default:
					break;
				}
			}

			border(boid);
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