#ifndef FLOCK_CLASS_H
#define FLOCK_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <chrono>
#include <numeric>
#include <string>

#include "Boid.h"
#include "ResourceManager.h"

class Flock
{
private:
	static std::mt19937 s_mt;
	static std::uniform_real_distribution<float> s_rX;
	static std::uniform_real_distribution<float> s_rY;

	const int dataSize{ 7 };

	std::vector<Boid> m_flock{};
	std::vector<int> m_groupSize{};

	int m_width{};
	int m_height{};
	float m_aspect{};

	std::chrono::high_resolution_clock::time_point m_lastUpdate{ std::chrono::high_resolution_clock::now() };
	double m_updatePeriod{ 1.0 / 120.0 };

	float* m_data{ nullptr };

	std::string m_shaderName{ "flock" };
	GLuint m_VAO;
	GLuint m_VBO;


	void updateData();


	void seekPosition	(Boid& boid, float weight, const glm::vec2& targetPosition);

	void fleePosition	(Boid& boid, float weight, const glm::vec2& targetPosition);

	void pursueClosestType(Boid& boid, float weight, Boid::Type type);

	void evadeClosestType(Boid& boid, float weight, Boid::Type type);


	void seperate(Boid& boid);
	void align(Boid& boid);
	void cohesion(Boid& boid);

	void border(Boid& boid);

	void resizeData();

public:
	Flock(int width, int height);
	~Flock();

	void addGroup(int amount, const Boid::BoidVariables& variables);
	bool run();
	void render();

	std::size_t getByteSize() const { return dataSize * getAmount() * sizeof(float); }
	int getAmount() const { return std::accumulate(m_groupSize.begin(), m_groupSize.end(), 0); }
};

#endif

