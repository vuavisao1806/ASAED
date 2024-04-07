#ifndef HEADER_ASAED_MATH_RANDOM_HPP
#define HEADER_ASAED_MATH_RANDOM_HPP

#include <random>

class Random {
private:
	std::mt19937 m_generator;

public:
	Random();

private:
	Random(const Random&) = delete;
	Random& operator=(const Random&) = delete;

public:
	/** Seed the generator */
	void seed(int v);

	/**  Generate random integers between [u, v) */
	int rand(int u, int v);

	/**  Generate random floats between [u, v) */
	float randf(float v);
	
	/**  Generate random floats between [u, v) */
	float randf(float u, float v);

	bool test_lucky(float percent = 5.0f);
};

extern Random g_game_random;

#endif