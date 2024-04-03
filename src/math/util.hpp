#ifndef HEADER_ASAED_MATH_UTIL_HPP
#define HEADER_ASAED_MATH_UTIL_HPP

#include <cmath>

class Vector;

namespace math {
	float degrees_to_radians(float angle);
	float randians_to_degrees(float angle);

	/** sin by degree (Actually, a degree is more familiar than radian) */
	float sin_degree(float angle);
	
	/** cos by degree (Actually, a degree is more familiar than radian) */
	float cos_degree(float angle);
	
	/** angle of vector (like normal) */
	float angle(const Vector& vector);
} // namespace math

#endif