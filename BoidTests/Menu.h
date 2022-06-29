#ifndef BOID_MENU_H
#define BOID_MENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

#include "Slider.h"
#include "Boid.h"
#include "ResourceManager.h"

class Menu
{
private:
	enum class sliderType
	{
		seperation,
		alignment,
		cohesion,
		pursue_evade,

		max_types
	};

	int m_width;
	int m_height;
	float m_aspect{};

	const int m_dataSize{ 5 };

	std::vector<std::array<Slider, static_cast<int>(sliderType::max_types)>> m_sliders{};

	int m_graphicDataSize{ 16 };
	float* m_graphicData{ nullptr };
	float* m_sliderData{ nullptr };


	std::string m_shaderName{ "menu" };
	GLuint m_VAO{};
	GLuint m_VBO{};

	Slider* m_currentActive{ nullptr };

	void updateGraphicData();
	void updateSliderData();

	void updateSliderVariables();

	void resizeData();

public:
	Menu(int width, int height);
	~Menu();
	void render();

	void addSliderGroup(glm::vec2 position, float length, float height, float seperation);

	void MoveActiveSlider(float mouseX);
	void setActiveSlider(glm::vec2 mousePos);

	void resetActiveSlider() { m_currentActive = nullptr; }
	std::size_t getByteSize() const { return m_dataSize * getAmount() * sizeof(float); }
	int getAmount() const { return 4 * static_cast<int>(m_sliders.size()); }
	bool sliderActive() { return m_currentActive; }
};

#endif