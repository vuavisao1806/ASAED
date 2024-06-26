#include "math/random.hpp"

#include <ctime>
#include <limits>

Random g_game_random;

Random::Random() :
	m_generator()
{
	seed(0);
}

void Random::seed(int v) {
	if (v <= 0) {
		v = static_cast<int>(std::time(nullptr));
	}
	m_generator.seed(v);

}

int Random::rand(int u, int v) {
	return std::uniform_int_distribution<int>(u, v)(m_generator);
}

float Random::randf(float v) {
	return randf(0.0f, v);
}

float Random::randf(float u, float v) {
	return std::uniform_real_distribution<float>(u, v)(m_generator);
}

bool Random::test_lucky(float percent) {
	return randf(0.0f, 100.0f) <= percent;
}