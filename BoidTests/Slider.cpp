#include "Slider.h"

Slider::Slider()
{ }

Slider::Slider(const glm::vec2& position, float length, float height)
	:m_position{ position },
	m_length{ length },
	m_height{ height }
{ }

Slider::Slider(const Slider& slider)
{
	m_position = slider.m_position;
	m_length = slider.m_length;
	m_height = slider.m_height;
	m_sliderPosition = slider.m_sliderPosition;
	m_percentage = slider.m_percentage;
}

void Slider::operator=(const Slider& slider)
{
	m_position = slider.m_position;
	m_length = slider.m_length;
	m_height = slider.m_height;
	m_sliderPosition = slider.m_sliderPosition;
	m_percentage = slider.m_percentage;
}

void Slider::updatePosition(float cursorX)
{
	float normalPos{ cursorX - m_position.x + (m_length / 2.0f) };
	if (normalPos > m_length)
	{
		m_sliderPosition = m_length;
	}
	else if (normalPos < 0.0f)
	{
		m_sliderPosition = 0.0f;
	}
	else
	{
		m_sliderPosition = normalPos;
	}
	m_percentage = m_sliderPosition / m_length;
}

bool Slider::mouseOver(glm::vec2 mouseposition) const
{
	//return true;
	return mouseposition.y <= m_position.y + m_height &&
		mouseposition.y >= m_position.y - m_height &&
		mouseposition.x <= m_position.x + (m_length) / 2.0f &&
		mouseposition.x >= m_position.x - (m_length) / 2.0f;
}