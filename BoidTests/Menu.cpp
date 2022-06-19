#include "Menu.h"

void Menu::addSlider(Boid::Type type, const glm::vec2& position, float length, float height)
{
	++m_numberOfGroups;
	m_data = new float[5 * m_numberOfGroups];
	Slider(position, length, height);
	m_weightSliders[type] = SliderGroup{ Slider{ position, length, height }, Slider{ glm::vec2(position.x, position.y - 3 * height) , length, height }, Slider{ glm::vec2(position.x, position.y - 6 * height), length, height } };
	updateData();
}