#include "Boid.h"

std::mt19937 Boid::m_rand{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_real_distribution<float> Boid::randomAngle{ 0.0f, 6.283185f };

std::vector<Boid>* Boid::m_flock{ nullptr };

int Boid::m_width{ 0 };
int Boid::m_height{ 0 };

// deafult constuctor sets boids initial position to (0,0)
Boid::Boid()
	:m_position{ glm::vec2{0.0f, 0.0f} },
	m_size{ 0.0f },
	m_type{ Type::prey }
{ }

// constuctor to set initial position and size of a boid and give the boid a random direction to face
Boid::Boid(const glm::vec2& position, Type type, float size)
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
	if (glm::length(m_acceleration) > m_maxForce)
	{
		//m_acceleration = glm::normalize(m_acceleration) * m_maxForce;
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

// apply an acceleration to go to a specific location
void Boid::seek(const glm::vec2& target)
{
	glm::vec2 steering = target - m_position;
	steering = glm::normalize(steering) * m_maxSpeed;

	steering -= m_velocity;

	if (glm::length(steering) > m_maxForce)
	{
		steering = glm::normalize(steering) * m_maxForce;
	}

	m_acceleration += steering;
}

void Boid::seek(Type target)
{
	glm::vec2 force{ 0.0f };
	bool found{ false };

	for (auto& boid : *m_flock)
	{
		if (boid.m_type == target)
		{
			float dist{ glm::distance(m_position, boid.m_position) };
			if (dist < glm::length(force) && dist < m_size * m_senseMult && dist > 0.0f)
			{
				force = boid.m_position;
			}
			else if (!found && dist < m_size * m_senseMult && dist > 0.0f)
			{
				force = boid.m_position;
				found = true;
			}
		}
	}

	if (found)
	{
		seek(force);
	}
}

void Boid::seek()
{
	glm::vec2 force{ 0.0f };
	bool found{ false };

	for (auto& boid : *m_flock)
	{
		float dist{ glm::distance(m_position, boid.m_position) };
		if (dist < glm::length(force) && dist < m_size * m_senseMult && dist > 0.0f)
		{
			force = boid.m_position;
		}
		else if (!found && dist < m_size * m_senseMult && dist > 0.0f)
		{
			force = boid.m_position;
			found = true;
		}
	}

	if (found)
	{
		seek(force);
	}
}

void Boid::flee(const glm::vec2& target)
{
	glm::vec2 steering = m_position - target;
	steering = glm::normalize(steering) * m_maxSpeed;

	steering -= m_velocity;

	if (glm::length(steering) > m_maxForce)
	{
		steering = glm::normalize(steering) * m_maxForce;
	}

	m_acceleration += steering;
}

void Boid::flee(Type target)
{
	glm::vec2 force{ 0.0f };
	bool found{ false };

	for (auto& boid : *m_flock)
	{
		if (boid.m_type == target)
		{
			float dist{ glm::distance(m_position, boid.m_position) };
			if (dist < glm::length(force) && dist < m_size * m_senseMult && dist > 0.0f)
			{
				force = boid.m_position;
			}
			else if (!found && dist < m_size * m_senseMult && dist > 0.0f)
			{
				force = boid.m_position;
				found = true;
			}
		}
	}

	if (found)
	{
		flee(force);
	}
}

void Boid::flee()
{
	glm::vec2 force{ 0.0f };
	bool found{ false };

	for (auto& boid : *m_flock)
	{
		float dist{ glm::distance(m_position, boid.m_position) };
		if (dist < glm::length(force) && dist < m_size * m_senseMult && dist > 0.0f)
		{
			force = boid.m_position;
		}
		else if (!found && dist < m_size * m_senseMult && dist > 0.0f)
		{
			force = boid.m_position;
			found = true;
		}
	}

	if (found)
	{
		flee(force);
	}
}



void Boid::pursue(Type target)
{
	glm::vec2 fPrediction{ 0.0f };
	bool found{ false };

	for (auto& boid : *m_flock)
	{
		if (boid.m_type == target)
		{
			float dist{ glm::distance(m_position, boid.m_position) };
			if (dist > 0.0f && dist <= m_senseMult * m_size && dist < glm::distance(m_position, fPrediction))
			{
				fPrediction = boid.m_position + boid.m_velocity * dist * m_maxForce;
				found = true;
			}
			else if (!found && dist > 0.0f && dist <= m_senseMult * m_size)
			{
				fPrediction = boid.m_position + boid.m_velocity * dist * m_maxForce;
				found = true;
			}
		}
		
	}

	if (found)
	{
		seek(fPrediction);
	}
}

void Boid::evade(Type target)
{
	glm::vec2 fPrediction{ 0.0f };
	bool found{ false };

	for (auto& boid : *m_flock)
	{
		if (boid.m_type == target)
		{
			float dist{ glm::distance(m_position, boid.m_position) };
			if (dist > 0.0f && dist <= m_senseMult * m_size && dist < glm::distance(m_position, fPrediction))
			{
				fPrediction = boid.m_position + boid.m_velocity * dist * m_maxForce;
				found = true;
			}
			else if (!found && dist > 0.0f && dist <= m_senseMult * m_size)
			{
				fPrediction = boid.m_position + boid.m_velocity * dist * m_maxForce;
				found = true;
			}
		}
	}

	if (found)
	{
		flee(fPrediction);
	}
}

void Boid::seperate(float weight)
{
	float desireSeperation{ m_seperateMult * m_size };
	glm::vec2 steering{ 0.0f };
	int count{ 0 };
	for (auto& boid : *m_flock)
	{
		float dist{ glm::distance(m_position, boid.m_position) };
		if (dist > 0.0f && dist < desireSeperation)
		{
			glm::vec2 direction{ m_position - boid.m_position };
			direction = glm::normalize(direction) / dist;
			steering += direction;
			++count;
		}
	}

	if (count > 0)
	{
		steering = glm::normalize(steering) * m_maxSpeed;
		steering -= m_velocity;

		if (glm::length(steering) > m_maxForce)
		{
			steering = glm::normalize(steering) * m_maxForce;
		}

		m_acceleration += weight * steering;
	}
}

// apply an acceleration to align the velocity/direction of all nearby boids
void Boid::align(float weight)
{
	;
	glm::vec2 steering{ 0.0f };
	int count{ 0 };
	for (auto& boid : *m_flock)
	{
		float dist{ glm::distance(m_position, boid.m_position) };
		if (dist > 0.0f && dist <= m_senseMult * m_size)
		{
			steering += boid.m_velocity;
			++count;
		}
	}

	if (count > 0)
	{
		steering = glm::normalize(steering) * m_maxSpeed;
		steering -= m_velocity;

		if (glm::length(steering) > m_maxForce)
		{
			steering = glm::normalize(steering) * m_maxForce;
		}

		m_acceleration += weight * steering;
	}
}

// apply an acceleration to make all boids want to stay together
void Boid::cohesion(float weight)
{
	glm::vec2 steering{ 0.0f };
	int count{ 0 };
	for (auto& boid : *m_flock)
	{
		float dist{ glm::distance(m_position, boid.m_position) };
		if (dist > 0.0f && dist <= m_senseMult * m_size)
		{
			steering += boid.m_position;
			++count;
		}
	}

	if (count > 0)
	{
		steering /= static_cast<float>(count);
		seek(steering);
	}
}



void Boid::borderLoop()
{
	if (m_position.x < -m_size)
	{
		m_position.x = static_cast<float>(m_width) + m_size;
	}
	if (m_position.x > static_cast<float>(m_width) + m_size)
	{
		m_position.x = -m_size;
	}
	if (m_position.y < -m_size)
	{
		m_position.y = static_cast<float>(m_height) + m_size;
	}
	if (m_position.y > static_cast<float>(m_height) + m_size)
	{
		m_position.y = -m_size;
	}
}

void Boid::setBorder(int width, int height)
{
	m_width = width;
	m_height = height;
}

void Boid::setFlock(std::vector<Boid>* flock)
{
	if (m_flock == flock)
	{
		return;
	}

	delete m_flock;
	m_flock = flock;
}