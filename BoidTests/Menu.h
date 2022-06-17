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
	std::map<Boid::Type, SliderGroup> m_weightSliders;
	float* m_data;
public:
	void addSlider(const glm::vec2& position, float lenght, float height);
};

#endif