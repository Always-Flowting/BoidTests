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
	struct BoidVariables
	{
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
		sight_distance,
		seperation_distance,

		max_types
	};

	using variableSliders_t = std::array<Slider, static_cast<int>(SliderType::max_types)>;

private:
	// static random variables for initil angles
	static std::mt19937 s_mt;
	static std::uniform_real_distribution<float> s_rAngle;

	static BoidVariables s_variables;
	static variableSliders_t s_sliders;

	// boid movement variables
	glm::vec2 m_position{ 0.0f };
	glm::vec2 m_velocity{ 0.0f };
	glm::vec2 m_acceleration{ 0.0f };

public:
	Boid();
	Boid(const glm::vec2& position);
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

	static void setVariables(const BoidVariables& variables) { s_variables = variables; }
	static void setSliders(const variableSliders_t& sliders) { s_sliders = sliders; }
	
	static const BoidVariables& getCVariables() { return s_variables; }
	static BoidVariables& getVariables() { return s_variables; }

	static const variableSliders_t& getCSliders() { return s_sliders; }
	static variableSliders_t& getSliders() { return s_sliders; }

};

#endif