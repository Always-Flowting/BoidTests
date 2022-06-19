#include "Boid.h"

std::mt19937 Boid::m_rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_real_distribution<float> Boid::randomAngle{ 0.0f, 6.283185f };

std::map<Boid::Type, glm::vec3> Boid::m_colour{
	{Boid::Type::prey1, glm::vec3{0.23f, 0.79f, 0.42f}},
	{Boid::Type::prey2, glm::vec3{0.09f, 0.79f, 0.51f}},
	{Boid::Type::prey3, glm::vec3{0.91f, 0.8f, 0.32f}},
	{Boid::Type::predator1, glm::vec3{0.73f, 0.02f, 0.11f}},
	{Boid::Type::predator2, glm::vec3{0.61f, 0.09f, 0.36f}},
	{Boid::Type::predator3, glm::vec3{0.08f, 0.06f, 0.25f}}
};

std::map<Boid::Type, Boid::TypeVars> Boid::m_typeVariables{};

// default constructor for a boid object
Boid::Boid()
	:m_position{ glm::vec2{0.0f, 0.0f} },
	m_size{ 0.0f },
	m_type{ Type::prey1 }
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
void Boid::update()
{
	if (glm::length(m_acceleration) > m_typeVariables[m_type].maxAcceleration)
	{
		m_acceleration = glm::normalize(m_acceleration) * m_typeVariables[m_type].maxAcceleration;
	}
	m_velocity += m_acceleration;

	if (glm::length(m_velocity) > m_typeVariables[m_type].maxVelocity)
	{
		m_velocity = glm::normalize(m_velocity) * m_typeVariables[m_type].maxVelocity;
	}
	m_position += m_velocity;

	m_acceleration = glm::vec2{ 0.0f };
}