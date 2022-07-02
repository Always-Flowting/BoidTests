#include "Menu.h"

Menu::Menu(int width, int height)
	:m_width{ width },
	m_height{ height }
{
	m_aspect = m_width / static_cast<float>(m_height);

	ResourceManager::loadShader("slider.vert", "slider.frag", "slider.geom", m_shaderName);

	glm::mat4 projection = glm::ortho(0.0f, m_aspect, 0.0f, 1.0f);
	ResourceManager::getShader(m_shaderName).setMat4("proj", projection);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	//float data[10]{ 0.5f, 0.5f, 0.5f, 0.4f, 0.25f, 0.25f, 0.02f, 0.02f, 0.2f, 0.01f };
	//float data[10]{ 0.5f, 0.5f, 0.25f, 0.02f, 0.5f, 0.5f, 0.4f, 0.25f, 0.02f, 0.4f };
	//float data[5]{ 0.5f, 0.5f, 0.25f, 0.02f, 0.5f };


	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	m_data = new float[5 * static_cast<int>(Boid::SliderType::max_types)];

	glBufferData(GL_ARRAY_BUFFER, static_cast<int>(Boid::SliderType::max_types) * 5 * sizeof(float), m_data, GL_DYNAMIC_DRAW);

	updateAllData();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(1 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));


	glBindVertexArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

Menu::~Menu()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void Menu::render()
{
	ResourceManager::getShader(m_shaderName).activate();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, static_cast<int>(Boid::SliderType::max_types));
	glBindVertexArray(0);
}

void Menu::MoveActiveSlider(float mouseX)
{
	if (m_activeSlider)
	{
		m_activeSlider->updatePosition(mouseX);
	}
	updateAllData();
}

void Menu::setActiveSlider(glm::vec2 mousePos)
{
	for (Slider& slider : Boid::getSliders())
	{
		if (slider.mouseOver(mousePos))
		{
			m_activeSlider = &slider;
			return;
		}
	}
	m_activeSlider = nullptr;
}

void Menu::updateAllData()
{
	//int offset{ static_cast<int>(Boid::SliderType::max_types) };
	int count{ 0 };

	for (const Slider& slider : Boid::getCSliders())
	{
		m_data[count++] = m_aspect * slider.getPosition().x / m_width;
		std::cout << m_data[count - 1] << '\n';

		m_data[count++] = slider.getPosition().y / m_height;
		std::cout << m_data[count - 1] << '\n';

		m_data[count++] = slider.getLength() / m_height;
		std::cout << m_data[count - 1] << '\n';

		m_data[count++] = slider.getHeight() / m_height;
		std::cout << m_data[count - 1] << '\n';

		m_data[count++] = slider.getPercentage();
		std::cout << m_data[count - 1] << '\n' << '\n';
	}



	ResourceManager::getShader(m_shaderName).activate();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, 5 * static_cast<int>(Boid::SliderType::max_types) * sizeof(float), m_data);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int>(Boid::SliderType::max_types) * 5 * sizeof(float), m_data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Menu::updateSliderData()
{
	int count{ 0 };
	float data[static_cast<int>(Boid::SliderType::max_types)];
	for (const Slider& slider : Boid::getCSliders())
	{
		data[count++] = slider.getPercentage();
	}
	ResourceManager::getShader(m_shaderName).activate();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, static_cast<int>(Boid::SliderType::max_types) * 4 * sizeof(float), static_cast<int>(Boid::SliderType::max_types) * sizeof(float), data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}