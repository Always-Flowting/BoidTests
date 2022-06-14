#ifndef BOID_CLASS_H
#define BOID_CLASS_H

#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

class Boid
{
public:
	enum class Type
	{
		prey,
		predator,

		max_types
	};

private:
	// static random variables for initil angles
	static std::mt19937 m_rand;
	static std::uniform_real_distribution<float> randomAngle;

	// have all bois access to a list of their flock
	static std::vector<Boid>* m_flock;

	// size of the screen containing all boids
	static int m_width;
	static int m_height;

	// boid movement variables
	glm::vec2 m_position;
	glm::vec2 m_velocity{ 0.0f };
	glm::vec2 m_acceleration{ 0.0f };

	float m_maxSpeed{ 2.0f };
	// maximum acceleration for turning
	float m_maxForce{ 0.04f };

	// size of the boid
	float m_size;
	float m_senseMult{ 25.0f };
	float m_seperateMult{ 12.5f };

	Type m_type;

	// will move the boid across borders
	void borderLoop();

public:
	Boid();
	Boid(const glm::vec2& position, Type type, float size);


	void seek(const glm::vec2& target);
	void seek(Type target);
	void seek();

	void flee(const glm::vec2& target);
	void flee(Type target);
	void flee();

	void align(float weight);
	void seperate(float weight);
	void cohesion(float weight);

	void pursue(Type target);

	void evade(Type target);

	void update();

	const glm::vec2& getPosition() const { return m_position; }
	const glm::vec2& getVelocity() const { return m_velocity; }
	float getAngle() const { return glm::atan(m_velocity.y, m_velocity.x); }
	float getSize() const { return m_size; }
	Type getType() const { return m_type; }

	// set the screen borders for all boids starting at bottom left corner (0,0) to top right (width,height)
	static void setBorder(int width, int height);
	static void setFlock(std::vector<Boid>* flock);
};

#endif