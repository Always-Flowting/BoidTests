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
	steering = glm::normalize(steering) * boid.getVariables().maxVelocity;

	steering -= boid.getVelocity();

	boid.addAcceleration(weight * steering);
}

/*
/ All fleeing based functions
*/

// flee from a set position
void Flock::fleePosition(Boid& boid, float weight, const glm::vec2& targetPosition)
{
	glm::vec2 steering = boid.getPosition() - targetPosition;
	steering = glm::normalize(steering) * boid.getVariables().maxVelocity;

	steering -= boid.getVelocity();

	boid.addAcceleration(weight * steering);
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
		float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
		if (dist > 0.0f && dist <= Boid::getCSliders()[static_cast<int>(Boid::SliderType::seperation_distance)].getPercentage())
		{
			steering += glm::normalize(boid.getPosition() - other.getPosition()) / dist;
			found = true;
		}
	}

	if (found)
	{
		steering = glm::normalize(steering) * boid.getVariables().maxVelocity;
		steering -= boid.getVelocity();

		boid.addAcceleration(Boid::getCSliders()[static_cast<int>(Boid::SliderType::seperation)].getPercentage() * steering);
	}
}

void Flock::align(Boid& boid)
{
	glm::vec2 direction{ 0.0f };
	bool found{ false };

	for (auto& other : m_flock)
	{
		float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
		if (dist > 0.0f && dist <= Boid::getCSliders()[static_cast<int>(Boid::SliderType::sight_distance)].getPercentage())
		{
			direction += other.getVelocity();
			found = true;
		}
	}

	if (found)
	{
		direction = glm::normalize(direction) * boid.getVariables().maxVelocity;
		direction -= boid.getVelocity();

		boid.addAcceleration(Boid::getCSliders()[static_cast<int>(Boid::SliderType::alignment)].getPercentage() * direction);
	}
}

void Flock::cohesion(Boid& boid)
{
	glm::vec2 target{ 0.0f };
	int count{ 0 };

	for (auto& other : m_flock)
	{
		float dist{ glm::distance(boid.getPosition(), other.getPosition()) };
		if (dist > 0.0f && dist <= Boid::getCSliders()[static_cast<int>(Boid::SliderType::sight_distance)].getPercentage())
		{
			target += other.getPosition();
			++count;
		}
	}

	if (count > 0)
	{
		target /= static_cast<float>(count);
		seekPosition(boid, Boid::getCSliders()[static_cast<int>(Boid::SliderType::cohesion)].getPercentage(), target);
	}
}



void Flock::border(Boid& boid)
{
	if (boid.getPosition().x < -boid.getVariables().size)
	{
		boid.setPosition(glm::vec2(static_cast<float>(m_width) + boid.getVariables().size, boid.getPosition().y));
	}
	if (boid.getPosition().x > static_cast<float>(m_width) + boid.getVariables().size)
	{
		boid.setPosition(glm::vec2(-boid.getVariables().size, boid.getPosition().y));
	}
	if (boid.getPosition().y < -boid.getVariables().size)
	{
		boid.setPosition(glm::vec2(boid.getPosition().x, static_cast<float>(m_height) + boid.getVariables().size));
	}
	if (boid.getPosition().y > static_cast<float>(m_height) + boid.getVariables().size)
	{
		boid.setPosition(glm::vec2(boid.getPosition().x, -boid.getVariables().size));
	}
}

void Flock::updateData()
{
	for (int i{ 0 }; i < getAmount(); ++i)
	{
		m_data[(dataSize * i)]		= (m_aspect * m_flock[i].getPosition().x / m_width);
		m_data[(dataSize * i) + 1]	= (m_flock[i].getPosition().y / m_height);
		m_data[(dataSize * i) + 2]	= m_flock[i].getAngle();
		m_data[(dataSize * i) + 3]	= (m_flock[i].getVariables().size / m_height);
		m_data[(dataSize * i) + 4]	= m_flock[i].getVariables().r;
		m_data[(dataSize * i) + 5]	= m_flock[i].getVariables().g;
		m_data[(dataSize * i) + 6]	= m_flock[i].getVariables().b;
	}



	ResourceManager::getShader(m_shaderName).activate();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, getByteSize(), m_data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}




Flock::Flock(int width, int height)
	:m_width{ width },
	m_height{ height }
{
	ResourceManager::loadShader("boid.vert", "boid.frag", "wireBoid.geom", m_shaderName);

	m_aspect = m_width / static_cast<float>(m_height);

	glm::mat4 projection = glm::ortho(0.0f, m_aspect, 0.0f, 1.0f);

	ResourceManager::getShader(m_shaderName).setMat4("proj", projection);

	s_rX = std::uniform_real_distribution<float>{ 0.0f, static_cast<float>(m_width) };
	s_rY = std::uniform_real_distribution<float>{ 0.0f, static_cast<float>(m_height) };

	glGenBuffers(1, &m_VBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

Flock::~Flock()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	delete[] m_data;
}

void Flock::addBoids(int amount)
{
	//glm::vec2 center{ m_width / 2.0f, m_height / 2.0f };
	for (int boids{ 0 }; boids < amount; ++boids)
	{
		m_flock.push_back(Boid(glm::vec2{ s_rX(s_mt), s_rY(s_mt) }));
	}

	resizeData();
	updateData();

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, getByteSize(), m_data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Flock::resizeData()
{
	delete[] m_data;
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

void Flock::render()
{
	glLineWidth(2.0f);
	ResourceManager::getShader(m_shaderName).activate();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, getAmount());
	glBindVertexArray(0);
}