#ifndef BOID_SLIDER_CLASS_H
#define BOID_SLIDER_CLASS_H

#include <glm/glm.hpp>

#include <iostream>

class Slider
{
private:
	glm::vec2 m_position{ 0.0f };
	float m_length{ 0.0f };
	float m_height{ 0.0f };
	float m_sliderPosition{ 0.0f };
	float m_mult{ 0.0f };
	float m_percentage{ 0.0f };

public:
	Slider();
	Slider(const glm::vec2& position, float length, float height, float mult);
	Slider(const Slider& slider);

	void operator=(const Slider& slider);

	void updatePosition(float cursorX);

	bool mouseOver(glm::vec2 mouseposition) const;

	const glm::vec2& getPosition() const { return m_position; }
	float getLength() const { return m_length; }
	float getHeight() const { return m_height; }
	float getSliderPosition() const { return m_sliderPosition; }
	float getPercentage() const { return m_percentage; }
	float getMultiplied() const { return m_mult * m_percentage; }
	float getMult() const { return m_mult; }
};

#endif