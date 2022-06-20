#include "Flock.h"

std::mt19937 Flock::s_mt{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
std::uniform_real_distribution<float> Flock::s_rX;
std::uniform_real_distribution<float> Flock::s_rY;

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
		fleePosition(boid, weight, fPrediction);
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
	if (boid.getPosition().x < -boid.getGroupVariables().size)
	{
		boid.setPosition(glm::vec2(static_cast<float>(m_width) + boid.getGroupVariables().size, boid.getPosition().y));
	}
	if (boid.getPosition().x > static_cast<float>(m_width) + boid.getGroupVariables().size)
	{
		boid.setPosition(glm::vec2(-boid.getGroupVariables().size, boid.getPosition().y));
	}
	if (boid.getPosition().y < -boid.getGroupVariables().size)
	{
		boid.setPosition(glm::vec2(boid.getPosition().x, static_cast<float>(m_height) + boid.getGroupVariables().size));
	}
	if (boid.getPosition().y > static_cast<float>(m_height) + boid.getGroupVariables().size)
	{
		boid.setPosition(glm::vec2(boid.getPosition().x, -boid.getGroupVariables().size));
	}
}

void Flock::updateData()
{
	for (int i{ 0 }; i < getAmount(); ++i)
	{
		m_data[(dataSize * i)]		= (2.0f * m_aspect * m_flock[i].getPosition().x / m_width) - m_aspect;
		m_data[(dataSize * i) + 1]	= (2.0f * m_flock[i].getPosition().y / m_height) - 1.0f;
		m_data[(dataSize * i) + 2]	= m_flock[i].getAngle();
		m_data[(dataSize * i) + 3]	= (2.0f * m_flock[i].getGroupVariables().size / m_height);
		m_data[(dataSize * i) + 4]	= m_flock[i].getGroupColour().r;
		m_data[(dataSize * i) + 5]	= m_flock[i].getGroupColour().g;
		m_data[(dataSize * i) + 6]	= m_flock[i].getGroupColour().b;
	}
}




Flock::Flock(int width, int height)
	:m_width{ width },
	m_height{ height }
{
	m_aspect = m_width / static_cast<float>(m_height);
	s_rX = std::uniform_real_distribution<float>{ 0.0f, static_cast<float>(m_width) };
	s_rY = std::uniform_real_distribution<float>{ 0.0f, static_cast<float>(m_height) };
}

Flock::~Flock()
{
	delete[] m_data;
}

void Flock::addGroup(int amount, Boid::Type type, const glm::vec3& colour, const Boid::BoidVariables& variables)
{
	static int group{ 0 };

	Boid::setGroupType(group, type);
	Boid::setGroupColour(group, colour);
	Boid::setGroupVariables(group, variables);

	m_groupSize.push_back(amount);
	//glm::vec2 center{ m_width / 2.0f, m_height / 2.0f };
	for (int boids{ 0 }; boids < amount; ++boids)
	{
		m_flock.push_back(std::move(Boid(glm::vec2{s_rX(s_mt), s_rY(s_mt)}, group)));
	}

	++group;
}

void Flock::resizeData()
{
	m_data = new float[dataSize * getAmount()];
}

// update the acceleration of boid depending on current behaviour then update the velocity and position. return true if boids were upated
bool Flock::run()
{
	if (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_lastUpdate).count() >= m_updatePeriod)
	{
		for (auto& boid : m_flock)
		{
			seperate(boid);
			align(boid);
			cohesion(boid);

			if (boid.getGroupType() == Boid::Type::prey)
			{
				evadeClosestType(boid, 2.5f, Boid::Type::predator);
			}
			else if (boid.getGroupType() == Boid::Type::predator)
			{
				pursueClosestType(boid, 4.5f, Boid::Type::prey);
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