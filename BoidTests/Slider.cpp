#include "Slider.h"

Slider::Slider(const glm::vec2& position, float length)
	:m_position{ position },
	m_length{ length },
	m_sliderPosition{ length }
{ }