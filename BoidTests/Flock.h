#ifndef FLOCK_CLASS_H
#define FLOCK_CLASS_H

#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <array>
#include <numeric>

#include "Boid.h"
#include "ResourceManager.h"

class Flock
{
private:
	std::vector<Boid> m_flock{};
	std::vector<int> m_groupSize{};

	int m_width{};
	int m_height{};

	std::chrono::high_resolution_clock::time_point m_lastUpdate{ std::chrono::high_resolution_clock::now() };
	double m_updatePeriod{ 1.0 / 60.0 };

	float* m_data{ nullptr };


	void updateData();


	void seekPosition	(Boid& boid, float weight, const glm::vec2& targetPosition);

	void fleePosition	(Boid& boid, float weight, const glm::vec2& targetPosition);

	void pursueClosestType(Boid& boid, float weight, Boid::Type type);

	void evadeClosestType(Boid& boid, float weight, Boid::Type type);


	void seperate(Boid& boid);
	void align(Boid& boid);
	void cohesion(Boid& boid);

	void border(Boid& boid);

public:
	Flock(int width, int height);
	~Flock();

	void addGroup
	bool run();

	std::size_t getByteSize() const { return 7 * (std::accumulate(m_flockSize.begin(), m_flockSize.end(), 0)) * sizeof(float); }
	int getAmount() const { return std::accumulate(m_flockSize.begin(), m_flockSize.end(), 0); }
	float* getData() { return m_data; }
};

#endif

