#include "Boid.h"

std::mt19937 Boid::m_rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_real_distribution<float> Boid::randomAngle{ 0.0f, 6.283185f };

// deafult constuctor sets boids initial position to (0,0)
Boid::Boid()
	:m_position{ glm::vec2{0.0f, 0.0f} },
	m_type{ Type::prey }
{ }

// constuctor to set initial position and size of a boid and give the boid a random direction to face
Boid::Boid(const glm::vec2& position, Type type, float size)
	:m_position{ position },
	m_type{ type }
{
	float angle{ randomAngle(m_rand) };
	m_velocity = glm::vec2(glm::cos(angle), glm::sin(angle));
}

// upate the boids velocity and position then reset the acceleration
void Boid::update()
{
	if (glm::length(m_acceleration) > m_maxForce)
	{
		m_acceleration = glm::normalize(m_acceleration) * m_maxForce;
	}
	m_velocity += m_acceleration;

	if (glm::length(m_velocity) > m_maxSpeed)
	{
		m_velocity = glm::normalize(m_velocity) * m_maxSpeed;
	}
	m_position += m_velocity;

	borderLoop();

	m_acceleration = glm::vec2{ 0.0f };
}