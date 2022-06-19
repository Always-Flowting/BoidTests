#ifndef BOID_SLIDER_CLASS_H
#define BOID_SLIDER_CLASS_H

#include <glm/glm.hpp>

class Slider
{
private:
	glm::vec2 m_position;
	float m_length;
	float m_height;
	float m_sliderPosition;
	float m_percentage{ 1.0f };

public:
	Slider(const glm::vec2& position, float length, float height);

	const glm::vec2& getPosition() const { return m_position; }
	float getLength() const { return m_length; }
	float getHeight() const { return m_height; }
	float getSliderPosition() const { return m_sliderPosition; }
	float getPercentage() const { return m_percentage; }
};

#endif