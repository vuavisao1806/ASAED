#include "math/util.hpp"

#include "math/vector.hpp"

// It's good enough (maybe)
constexpr float PI = 3.14159265359f;
// lazyyyy
constexpr float HAFT_CYCLE = 180.0f;

namespace math {
	float degrees_to_radians(float angle) { return (angle * PI) / HAFT_CYCLE; }
	float randians_to_degrees(float angle) { return (angle / PI) * HAFT_CYCLE; }

	// https://stackoverflow.com/questions/31502120/sin-and-cos-give-unexpected-results-for-well-known-angles/31525208#31525208 for more details
	// I hate real numbers, this function can improve handling sin(x), cos(x)
	float sin_degree(float angle) {
		int quo;
		float x90 = std::remquof(std::fabs(angle), 90.0f, &quo);
		if (angle < 0.0f) {
			return -sin_degree(-angle);
		}
		switch (quo % 4) {
			case 0:
				// Use * 1.0 to avoid -0.0
				return std::sin(degrees_to_radians(x90) * 1.0f);
			case 1:
				// cos(90 - x) = sin(x)
				return std::cos(degrees_to_radians(x90));
			case 2:
				// sin(180 - x) = sin(x)
				return std::sin(degrees_to_radians(-x90) * 1.0f);
			case 3:
				// sin(x) = cos(90 - x) = .... 
				return -std::cos(degrees_to_radians(x90));
		}
		return 0.0; // funny! Compiler likes this
	}
	
	/** cos by degree (Actually, a degree is more familiar than radian) */
	float cos_degree(float angle) {
		return sin_degree(90.0f - fabs(angle));
	}
	
	/** angle of vector (like normal) */
	float angle(const Vector& vector) {
		// atan2 go go
		return randians_to_degrees(std::atan2(vector.y, vector.x));
	}
} // namespace math