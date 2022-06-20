#ifndef BOID_CLASS_H
#define BOID_CLASS_H

#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <map>
#include <array>

class Boid
{
public:
	enum class Type
	{
		prey,
		predator,

		max_types
	};

	struct BoidVariables
	{
		float maxAcceleration{};
		float maxVelocity{};
		float seperationDistance{};
		float sightDistance{};
		float size{};
		float seperation{};
		float alignment{};
		float cohesion{};
	};

private:
	// static random variables for initil angles
	static std::mt19937 s_mt;
	static std::uniform_real_distribution<float> s_rAngle;

	static std::map<int, Type> s_groupType;
	static std::map<int, glm::vec3> s_groupColour;
	static std::map<int, BoidVariables> s_groupVariables;

	// boid movement variables
	glm::vec2 m_position;
	glm::vec2 m_velocity{ 0.0f };
	glm::vec2 m_acceleration{ 0.0f };

	int m_group;

public:
	Boid();
	Boid(const glm::vec2& position, int group);

	void update();

	void addAcceleration(const glm::vec2& acceleration) { m_acceleration += acceleration; }
	void setAcceleration(const glm::vec2& acceleration) { m_acceleration = acceleration; }

	void setPosition(const glm::vec2& position) { m_position = position; }


	const glm::vec2& getPosition()		const { return m_position; }
	const glm::vec2& getVelocity()		const { return m_velocity; }
	const glm::vec2& getAcceleration()	const { return m_acceleration; }

	float getAngle() const { return glm::atan(m_velocity.y, m_velocity.x); }

	int getGroup() const { return m_group; }

	static void setGroupType(int group, Type type) { s_groupType[group] = type; }
	static void setGroupColour(int group, const glm::vec3& colour) { s_groupColour[group] = colour; }
	static void setGroupVariables(int group, const BoidVariables& variables) { s_groupVariables[group] = variables; }

	const glm::vec3& getGroupColour() const { return s_groupColour[m_group]; }
	Type getGroupType() const { return s_groupType[m_group]; }
	const BoidVariables& getGroupVariables() const { return s_groupVariables[m_group]; }
};

#endif