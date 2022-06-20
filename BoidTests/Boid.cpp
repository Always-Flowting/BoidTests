#include "Boid.h"

std::mt19937 Boid::s_mt{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_real_distribution<float> Boid::s_rAngle{ 0.0f, 6.283185f };

// default constructor for a boid object
Boid::Boid()
	:m_position{ glm::vec2{ 0.0f } },
	m_group{ 0 }
{ }

// constructor for a boid object will give the boid a random direction to face
Boid::Boid(const glm::vec2& position, int group)
	:m_position{ position },
	m_group{ group }
{
	float angle{ s_rAngle(s_mt) };
	m_velocity = glm::vec2(glm::cos(angle), glm::sin(angle));
}

// upate the boids velocity and position then reset the acceleration
void Boid::update()
{
	if (glm::length(m_acceleration) > s_groupVariables[m_group].maxAcceleration)
	{
		m_acceleration = glm::normalize(m_acceleration) * s_groupVariables[m_group].maxAcceleration;
	}
	m_velocity += m_acceleration;

	if (glm::length(m_velocity) > s_groupVariables[m_group].maxVelocity)
	{
		m_velocity = glm::normalize(m_velocity) * s_groupVariables[m_group].maxVelocity;
	}
	m_position += m_velocity;

	m_acceleration = std::move(glm::vec2{ 0.0f });
}