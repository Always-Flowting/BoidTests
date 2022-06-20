#include "Flock.h"

/*
/ All seeking based function
*/

// seek the position specified
void Flock::seekPosition(Boid& boid, float weight, const glm::vec2& targetPosition)
{
	glm::vec2 steering = targetPosition - boid.getPosition();
	steering = glm::normalize(steering) * boid.getGroupVariables().maxVelocity;

	steering -= boid.getVelocity();

	boid.addAcceleration(weight * steering);
}

void Flock::pursueClosestType(Boid& boid, float weight, Boid::Type type)
{
	glm::vec2 fPrediction{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		if (other.getGroupType() == type)
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist < boid.getGroupVariables().sightDistance && dist > 0.0f && (dist < glm::length(fPrediction) || !found))
			{
				fPrediction = other.getPosition() + other.getVelocity() * dist * other.getGroupVariables().maxAcceleration;
				found = true;
			}
		}

	}

	if (found)
	{
		seekPosition(boid, weight, fPrediction);
	}
}

/*
/ All fleeing based functions
*/

// flee from a set position
void Flock::fleePosition(Boid& boid, float weight, const glm::vec2& targetPosition)
{
	glm::vec2 steering = boid.getPosition() - targetPosition;
	steering = glm::normalize(steering) * boid.getGroupVariables().maxVelocity;

	steering -= boid.getVelocity();

	boid.addAcceleration(weight * steering);
}

void Flock::evadeClosestType(Boid& boid, float weight, Boid::Type type)
{
	glm::vec2 fPrediction{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		if (other.getGroupType() == type)
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist < boid.getGroupVariables().sightDistance && dist > 0.0f && (dist < glm::length(fPrediction) || !found))
			{
				fPrediction = other.getPosition() + other.getVelocity() * dist * other.getGroupVariables().maxAcceleration;
				found = true;
			}
		}

	}

	if (found)
	{
		seekPosition(boid, weight, fPrediction);
	}
}

/*
/ flocking behaviour implimentation
*/

// adds a force to the boid to seperate it a set distance from boids of the same type
void Flock::seperate(Boid& boid)
{
	glm::vec2 steering{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		if (boid.getGroup() == other.getGroup())
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist > 0.0f && dist <= boid.getGroupVariables().seperationDistance)
			{
				steering += glm::normalize(boid.getPosition() - other.getPosition()) / dist;
				found = true;
			}
		}
	}

	if (found)
	{
		steering = glm::normalize(steering) * boid.getGroupVariables().maxVelocity;
		steering -= boid.getVelocity();

		boid.addAcceleration(boid.getGroupVariables().seperation * steering);
	}
}

void Flock::align(Boid& boid)
{
	glm::vec2 direction{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		if (boid.getGroup() == other.getGroup())
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist > 0.0f && dist <= boid.getGroupVariables().sightDistance)
			{
				direction += other.getVelocity();
				found = true;
			}
		}
	}

	if (found)
	{
		direction = glm::normalize(direction) * boid.getGroupVariables().maxVelocity;
		direction -= boid.getVelocity();

		boid.addAcceleration(boid.getGroupVariables().alignment * direction);
	}
}

void Flock::cohesion(Boid& boid)
{
	glm::vec2 target{ 0.0f };
	int count{ 0 };

	for (auto& other : m_flock)
	{
		if (boid.getGroup() == other.getGroup())
		{
			float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
			if (dist > 0.0f && dist <= boid.getGroupVariables().sightDistance)
			{
				target += other.getPosition();
				++count;
			}
		}
	}

	if (count > 0)
	{
		target /= static_cast<float>(count);
		seekPosition(boid, boid.getGroupVariables().cohesion, target);
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

void Flock::updateData()
{
	float aspect{ m_width / static_cast<float>(m_height) };
	for (int i{ 0 }; i < getAmount(); ++i)
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

void Flock::init(int prey1Num, int prey2Num, int prey3Num, int pred1Num, int pred2Num, int pred3Num)
{
	m_flockSize[static_cast<int>(Boid::Type::prey1)] = prey1Num;
	m_flockSize[static_cast<int>(Boid::Type::prey2)] = prey2Num;
	m_flockSize[static_cast<int>(Boid::Type::prey3)] = prey3Num;
	m_flockSize[static_cast<int>(Boid::Type::predator1)] = pred1Num;
	m_flockSize[static_cast<int>(Boid::Type::predator2)] = pred2Num;
	m_flockSize[static_cast<int>(Boid::Type::predator3)] = pred3Num;

	m_flock.clear();
	m_flock.resize(getAmount());
	m_data = new float[7 * getAmount()];

	int count{ 0 };
	int set{ 0 };
	for (int size : m_flockSize)
	{
		for (int i{ 0 }; i < size; ++i)
		{
			if (set == 0)
			{
				m_flock[count] = Boid{ glm::vec2(m_width / 2.0f, m_height / 2.0f), 3.0f, Boid::Type::prey1 };
			}
			else if (set == 1)
			{
				m_flock[count] = Boid{ glm::vec2(m_width / 2.0f, m_height / 2.0f), 3.0f, Boid::Type::prey2 };
			}
			else if (set == 2)
			{
				m_flock[count] = Boid{ glm::vec2(m_width / 2.0f, m_height / 2.0f), 3.0f, Boid::Type::prey3 };
			}
			else if (set == 3)
			{
				m_flock[count] = Boid{ glm::vec2(2.0f, 2.0f), 4.0f, Boid::Type::predator1 };
			}
			else if (set == 4)
			{
				m_flock[count] = Boid{ glm::vec2(2.0f, 2.0f), 4.0f, Boid::Type::predator2 };
			}
			else
			{
				m_flock[count] = Boid{ glm::vec2(2.0f, 2.0f), 4.0f, Boid::Type::predator3 };
			}
			++count;
		}
		++set;
	}
	Boid::setVars(Boid::Type::prey1, 0.04f, 3.0f, 70.0f, 20.0f);
	Boid::setVars(Boid::Type::prey2, 0.04f, 3.0f, 70.0f, 20.0f);
	Boid::setVars(Boid::Type::prey3, 0.04f, 3.0f, 70.0f, 20.0f);
	Boid::setVars(Boid::Type::predator1, 0.06f, 3.0f, 90.0f, 40.0f);
	Boid::setVars(Boid::Type::predator2, 0.06f, 3.0f, 90.0f, 40.0f);
	Boid::setVars(Boid::Type::predator3, 0.06f, 3.0f, 90.0f, 40.0f);

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
	if (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_lastUpdate).count() >= m_updatePeriod)
	{
		processInput();
		for (auto& boid : m_flock)
		{
			seperate(boid, m_weights[boid.getType()].seperation);
			align(boid, m_weights[boid.getType()].alignment);
			cohesion(boid, m_weights[boid.getType()].cohesion);
			if (boid.getType() == Boid::Type::prey1)
			{
			}
			if (boid.getType() < Boid::Type::predator1)
			{
				evadeClosestType(boid, 3.0f);
			}
			if (boid.getType() > Boid::Type::prey3)
			{
				pursueClosestType(boid, 3.0f);
			}
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