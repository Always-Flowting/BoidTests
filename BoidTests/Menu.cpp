#include "Menu.h"

Menu::Menu(int width, int height)
	:m_width{ width },
	m_height{ height }
{

	ResourceManager::loadShader("slider.vert", "slider.frag", "slider.geom", m_shaderName);

	m_aspect = m_width / static_cast<float>(m_height);

	//glm::mat4 projection = glm::ortho(-m_aspect, m_aspect, -1.0f, 1.0f);
	glm::mat4 projection = glm::ortho(0.0f, m_aspect, 0.0f, 1.0f);

	ResourceManager::getShader(m_shaderName).setMat4("proj", projection);

	glGenBuffers(1, &m_VBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_sliders.push_back(std::array<Slider, static_cast<int>(sliderType::max_types)>{ Slider{ glm::vec2{600.0f, 337.5f}, 250.0f, 7.5f }, Slider{ glm::vec2{600.0f, 287.5f}, 250.0f, 7.5f }, Slider{ glm::vec2{600.0f, 237.5f}, 250.0f, 7.5f }, Slider{ glm::vec2{600.0f, 187.5f}, 250.0f, 7.5f } });
	m_sliders.push_back(std::array<Slider, static_cast<int>(sliderType::max_types)>{ Slider{ glm::vec2{900.0f, 337.5f}, 250.0f, 7.5f }, Slider{ glm::vec2{900.0f, 287.5f}, 250.0f, 7.5f }, Slider{ glm::vec2{900.0f, 237.5f}, 250.0f, 7.5f }, Slider{ glm::vec2{900.0f, 187.5f}, 250.0f, 7.5f } });
	glBufferData(GL_ARRAY_BUFFER, getByteSize(), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, m_graphicDataSize, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, m_graphicDataSize, (void*)8);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, m_graphicDataSize, (void*)12);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(getAmount() * m_graphicDataSize));


	glBindVertexArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);

	m_graphicData = new float[4 * getAmount()];
	updateGraphicData();
	m_sliderData = new float[getAmount()];
	updateSliderData();
}

Menu::~Menu()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	delete[] m_graphicData;
	delete[] m_sliderData;
}

void Menu::render()
{
	ResourceManager::getShader(m_shaderName).activate();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, getAmount());
	glBindVertexArray(0);
}

void Menu::MoveActiveSlider(float mouseX)
{
	if (m_currentActive)
	{
		m_currentActive->updatePosition(mouseX);
		updateSliderVariables();
		updateSliderData();
	}
}

void Menu::setActiveSlider(glm::vec2 mousePos)
{
	for (auto& group : m_sliders)
	{
		for (auto& slider : group)
		{
			if (slider.mouseOver(mousePos))
			{
				m_currentActive = &slider;
				return;
			}
		}
	}
	m_currentActive = nullptr;
}

void Menu::updateGraphicData()
{
	int count{ 0 };
	for (auto& group : m_sliders)
	{
		for (auto& slider : group)
		{
			m_graphicData[count++] = (m_aspect * slider.getPosition().x / m_width);
			m_graphicData[count++] = (slider.getPosition().y / m_height);
			m_graphicData[count++] = slider.getLength() / m_height;
			m_graphicData[count++] = slider.getHeight() / m_height;
		}
	}
	ResourceManager::getShader(m_shaderName).activate();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, getAmount() * m_graphicDataSize, m_graphicData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Menu::updateSliderData()
{
	int count{ 0 };
	for (auto& group : m_sliders)
	{
		for (auto& slider : group)
		{
			m_sliderData[count++] = slider.getPercentage();
		}
	}
	ResourceManager::getShader(m_shaderName).activate();
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, getAmount() * m_graphicDataSize, 4 * getAmount(), m_sliderData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Menu::updateSliderVariables()
{
	for (int slideGroup{ 0 }; slideGroup < static_cast<int>(m_sliders.size()); ++slideGroup)
	{
		for (int slideVar{ 0 }; slideVar < static_cast<int>(sliderType::max_types); ++slideVar)
		{
			sliderType typ{ static_cast<sliderType>(slideVar) };
			switch (typ)
			{
			case Menu::sliderType::seperation:
				Boid::getSGroupVariables(slideGroup).seperation = m_sliders[slideGroup][slideVar].getPercentage();
				break;
			case Menu::sliderType::alignment:
				Boid::getSGroupVariables(slideGroup).alignment = m_sliders[slideGroup][slideVar].getPercentage();
				break;
			case Menu::sliderType::cohesion:
				Boid::getSGroupVariables(slideGroup).cohesion = m_sliders[slideGroup][slideVar].getPercentage();
				break;
			case Menu::sliderType::pursue_evade:
				break;
			default:
				break;
			}
		}
	}
}