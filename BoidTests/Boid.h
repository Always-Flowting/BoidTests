#ifndef BOID_CLASS_H
#define BOID_CLASS_H

#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <map>

class Boid
{
public:
	enum class Type
	{
		prey,
		predator,

		max_types
	};

private:
	// static random variables for initil angles
	static std::mt19937 m_rand;
	static std::uniform_real_distribution<float> randomAngle;

	static std::map<Type, glm::vec3> m_colour;

	// boid movement variables
	glm::vec2 m_position;
	glm::vec2 m_velocity{ 0.0f };
	glm::vec2 m_acceleration{ 0.0f };

	float m_size;

	Type m_type;

public:
	Boid();
	Boid(const glm::vec2& position, float size, Type type);

	void addAcceleration(const glm::vec2& acceleration) { m_acceleration += acceleration; }
	void setAcceleration(const glm::vec2& acceleration) { m_acceleration = acceleration; }

	void setPosition(const glm::vec2& position) { m_position = position; }

	void update(float maxAcceleration, float maxVelocity);
	void update();

	const glm::vec2& getPosition()		const { return m_position; }
	const glm::vec2& getVelocity()		const { return m_velocity; }
	const glm::vec2& getAcceleration()	const { return m_acceleration; }
	float getAngle()				const { return glm::atan(m_velocity.y, m_velocity.x); }
	float getSize()					const { return m_size; }
	Type getType()					const { return m_type; }

	static void setColour(Type type, const glm::vec3& colour) { m_colour[type] = colour; }
	static const glm::vec3& getColour(Type type) { return m_colour[type]; }
};

#endif