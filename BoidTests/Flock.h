#ifndef FLOCK_CLASS_H
#define FLOCK_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <array>

#include "Boid.h"
#include "ResourceManager.h"

class Flock
{
public:
	struct BoidVariables
	{
		float seperation{};
		float alignment{};
		float cohesion{};
	};

private:
	enum class MouseAction
	{
		none,
		all_seek,
		all_flee,
		seek_in_range,
		flee_in_range,

		max_behaviours
	};

	std::vector<Boid> m_flock{};
	float* m_data{ nullptr };
	GLFWwindow* m_window;
	int m_width{};
	int m_height{};

	int m_preyAmount{};
	int m_predAmount{};

	double m_updatePeriod{ 1.0 / 60.0 };
	std::chrono::high_resolution_clock::time_point m_lastUpdate{ std::chrono::high_resolution_clock::now() };

	MouseAction m_currentMode{ MouseAction::all_seek };
	
	glm::vec2 m_mousePosition{ 0.0f };

	std::map<Boid::Type, BoidVariables> m_weights{};

	void updateData();

	void seekPosition	(Boid& boid, float weight, const glm::vec2& targetPosition);
	void seekClosest	(Boid& boid, float weight);
	void seekClosestType(Boid& boid, float weight, Boid::Type target);
	void rangeSeek		(Boid& boid, float weight, const glm::vec2& targetPosition);

	void fleePosition	(Boid& boid, float weight, const glm::vec2& targetPosition);
	void fleeClosest	(Boid& boid, float weight);
	void fleeClosestType(Boid& boid, float weight, Boid::Type target);
	void rangeFlee		(Boid& boid, float weight, const glm::vec2& targetPosition);

	void arrive(Boid& boid, float weight, const glm::vec2& target);


	void seperate(Boid& boid, float weight);
	void align(Boid& boid, float weight);
	void cohesion(Boid& boid, float weight);

	void eat(Boid& boid, float radius);

	void border(Boid& boid);

	void removeResize(std::vector<int>& toRemove);

public:
	Flock(GLFWwindow* window);
	~Flock();

	void init(int preyNum, int predNum = 0);
	void processInput();
	bool run();

	std::size_t getByteSize() const { return 7 * (m_preyAmount + m_predAmount) * sizeof(float); }
	int getAmount() const { return (m_preyAmount + m_predAmount); }
	float* getData() { return m_data; }

	void setVariables(Boid::Type type, float seperation, float alignment, float cohesion)
	{
		m_weights[type] = std::move(BoidVariables{ seperation, alignment, cohesion }) ;
	}
};

#endif

