#ifndef HEADER_ASAED_MATH_SIZEF_HPP
#define HEADER_ASAED_MATH_SIZEF_HPP

#include <iosfwd>

#include "math/vector.hpp"

class Size;

class Sizef final {
public :
	float width;
	float height;

public:
	Sizef();
	Sizef(float width_, float height_);
	
	Sizef(const Sizef& rhs) = default;
	Sizef& operator=(const Sizef& rhs) = default;

	Sizef(const Size& rhs);

	explicit Sizef(const Vector& v);

	Sizef& operator*=(float factor);
	Sizef& operator/=(float divisor);
	Sizef& operator+=(const Sizef& rhs);
	Sizef& operator-=(const Sizef& rhs);

	friend Sizef operator*(const Sizef& lhs, float factor);
	friend Sizef operator*(float factor, const Sizef& rhs);
	friend Sizef operator/(const Sizef& lhs, float divisor);
	friend Sizef operator+(const Sizef& lhs, const Sizef& rhs);
	friend Sizef operator-(const Sizef& lhs, const Sizef& rhs);
	friend bool operator==(const Sizef& lhs, const Sizef& rhs);
	friend bool operator!=(const Sizef& lhs, const Sizef& rhs);

	Vector as_vector() const;
	bool is_valid() const;
};

std::ostream& operator<<(std::ostream& s, const Sizef& size);

#endif