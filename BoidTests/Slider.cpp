#include "Slider.h"

Slider::Slider(const glm::vec2& position, float length, float height)
	:m_position{ position },
	m_length{ length },
	m_height{ height },
	m_sliderPosition{ length }
{ }