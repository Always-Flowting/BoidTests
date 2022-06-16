#include "Boid.h"

std::mt19937 Boid::m_rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_real_distribution<float> Boid::randomAngle{ 0.0f, 6.283185f };

std::map<Boid::Type, glm::vec3> Boid::m_colour{
	{Boid::Type::prey, glm::vec3{0.23f, 0.79f, 0.42f}},
	{Boid::Type::predator, glm::vec3{0.73f, 0.02f, 0.11f}}
};

// default constructor for a boid object
Boid::Boid()
	:m_position{ glm::vec2{0.0f, 0.0f} },
	m_size{ 0.0f },
	m_type{ Type::prey }
{ }

// constructor for a boid object will give the boid a random direction to face
Boid::Boid(const glm::vec2& position, float size, Type type)
	:m_position{ position },
	m_size{ size },
	m_type{ type }
{
	float angle{ randomAngle(m_rand) };
	m_velocity = glm::vec2(glm::cos(angle), glm::sin(angle));
}

// upate the boids velocity and position then reset the acceleration
void Boid::update(float maxAcceleration, float maxVelocity)
{
	if (glm::length(m_acceleration) > maxAcceleration)
	{
		m_acceleration = glm::normalize(m_acceleration) * maxAcceleration;
	}
	m_velocity += m_acceleration;

	if (glm::length(m_velocity) > maxVelocity)
	{
		m_velocity = glm::normalize(m_velocity) * maxVelocity;
	}
	m_position += m_velocity;

	m_acceleration = glm::vec2{ 0.0f };
}

void Boid::update()
{
	m_velocity += m_acceleration;
	m_position += m_velocity;
	m_acceleration = glm::vec2{ 0.0f };
}