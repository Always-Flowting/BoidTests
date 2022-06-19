#ifndef BOID_MENU_H
#define BOID_MENU_H

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <array>

#include "Slider.h"
#include "Boid.h"

class Menu
{
public:
	struct SliderGroup
	{
		Slider seperation;
		Slider alignment;
		Slider cohesion;
	};

private:
	static std::map<Boid::Type, SliderGroup> m_weightSliders;
	static int m_numberOfGroups;
	static float* m_data;

	static void updateData();
public:

	static void addSlider(Boid::Type type, const glm::vec2& position, float length, float height);
	static const SliderGroup& getSliderGroup(Boid::Type type) { return m_weightSliders[type]; }

	static float* getData() { return m_data; }
	static std::size_t getSize() { return 15 * m_numberOfGroups; }
};

#endif