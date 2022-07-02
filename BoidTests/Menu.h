#ifndef BOID_MENU_H
#define BOID_MENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

#include "Boid.h"
#include "Slider.h"
#include "ResourceManager.h"

class Menu
{
private:
	int m_width;
	int m_height;
	float m_aspect{};

	std::string m_shaderName{ "menu" };
	GLuint m_VAO{};
	GLuint m_VBO{};

	Slider* m_activeSlider{ nullptr };

	float* m_data{ nullptr };

	void updateSliderData();

public:
	Menu(int width, int height);
	~Menu();
	void render();

	void MoveActiveSlider(float mouseX);
	void setActiveSlider(glm::vec2 mousePos);

	void clearActiveSlider() { m_activeSlider = nullptr; }
	bool sliderActive() { return m_activeSlider; }
	void updateAllData();
};

#endif