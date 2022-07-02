#include "Boid.h"

std::mt19937 Boid::s_mt{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_real_distribution<float> Boid::s_rAngle{ 0.0f, 6.283185f };

Boid::BoidVariables Boid::s_variables{};
Boid::variableSliders_t Boid::s_sliders{};

// default constructor for a boid object
Boid::Boid()
{ }

// constructor for a boid object will give the boid a random direction to face
Boid::Boid(const glm::vec2& position)
	:m_position{ position }
{
	float angle{ s_rAngle(s_mt) };
	m_velocity = glm::vec2(glm::cos(angle), glm::sin(angle));
}

Boid::Boid(const Boid& boid)
{
	m_position = boid.m_position;
	m_velocity = boid.m_velocity;
	m_acceleration = boid.m_acceleration;
}

void Boid::operator=(const Boid& boid)
{
	m_position = boid.m_position;
	m_velocity = boid.m_velocity;
	m_acceleration = boid.m_acceleration;
}

// upate the boids velocity and position then reset the acceleration
void Boid::update()
{
	if (glm::length(m_acceleration) > s_variables.maxAcceleration)
	{
		m_acceleration = glm::normalize(m_acceleration) * s_variables.maxAcceleration;
	}
	m_velocity += m_acceleration;

	if (glm::length(m_velocity) > s_variables.maxVelocity)
	{
		m_velocity = glm::normalize(m_velocity) * s_variables.maxVelocity;
	}
	m_position += m_velocity;

	m_acceleration = std::move(glm::vec2{ 0.0f });
}