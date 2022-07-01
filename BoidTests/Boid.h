#ifndef BOID_CLASS_H
#define BOID_CLASS_H

#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <map>
#include <array>

#include "Slider.h"

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
		Type type{};
		float maxAcceleration{};
		float maxVelocity{};
		float size{};
		float r{};
		float g{};
		float b{};
	};

	enum class SliderType
	{
		seperation,
		alignment,
		cohesion,
		pursue_evade,
		sight_distance,
		seperation_distance,

		max_types
	};

private:
	// static random variables for initil angles
	static std::mt19937 s_mt;
	static std::uniform_real_distribution<float> s_rAngle;

	static std::map<int, BoidVariables> s_groupVariables;
	static std::map<int, std::array<Slider, static_cast<int>(SliderType::max_types)>> s_groupSlider;

	static int s_groupNumber;

	// boid movement variables
	glm::vec2 m_position;
	glm::vec2 m_velocity{ 0.0f };
	glm::vec2 m_acceleration{ 0.0f };

	int m_group;

public:
	Boid();
	Boid(const glm::vec2& position, int group);
	Boid(const Boid& boid);

	void operator=(const Boid& boid);

	void update();

	void addAcceleration(const glm::vec2& acceleration) { m_acceleration += acceleration; }
	void setAcceleration(const glm::vec2& acceleration) { m_acceleration = acceleration; }

	void setPosition(const glm::vec2& position) { m_position = position; }


	const glm::vec2& getPosition()		const { return m_position; }
	const glm::vec2& getVelocity()		const { return m_velocity; }
	const glm::vec2& getAcceleration()	const { return m_acceleration; }

	float getAngle() const { return glm::atan(m_velocity.y, m_velocity.x); }

	int getGroup() const { return m_group; }

	const BoidVariables& getVariables() const { return s_groupVariables[m_group]; }
	float getPercentage(SliderType type) const { return s_groupSlider[m_group][static_cast<int>(type)].getPercentage(); }

	static void setGroupVariables(int group, const BoidVariables& variables) { s_groupVariables[group] = std::move(variables); }
	static void setGroupSliders(int group, std::array<Slider, static_cast<int>(SliderType::max_types)>& sliders) { s_groupSlider[group] = sliders; }
	
	static BoidVariables& getGroupVariables(int group) { return s_groupVariables[group]; }
	static const std::array<Slider, static_cast<int>(SliderType::max_types)>& getGroupSliders(int group) { return s_groupSlider[group]; }

};

#endif