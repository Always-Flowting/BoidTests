#include "Slider.h"

Slider::Slider(const glm::vec2& position, float length, float height)
	:m_position{ position },
	m_length{ length },
	m_height{ height },
	m_sliderPosition{ length }
{ }

void Slider::updatePosition(float mouseX)
{
	float norm{ mouseX - m_position.x + (m_length /2.0f)};
	if (norm > m_length)
	{
		m_sliderPosition = m_length;
	}
	else if (norm < 0.0f)
	{
		m_sliderPosition = 0.0f;
	}
	else
	{
		m_sliderPosition = norm;
	}
	std::cout << m_sliderPosition << '\n';
}

bool Slider::mouseOver(glm::vec2 mouseposition)
{
	return mouseposition.y <= m_position.y + m_height &&
		mouseposition.y >= m_position.y - m_height &&
		mouseposition.x <= m_position.x + m_length / 2.0f &&
		mouseposition.x >= m_position.x - m_length / 2.0f;
}