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
private:
	float* m_data;

public:
	void render();
};

#endif