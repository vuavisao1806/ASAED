#include "math/vector.hpp"

#include <iostream>
#include <cmath>

#include "math/util.hpp"

Vector::Vector():
	x(0), 
	y(0)
{}

Vector::Vector(const Vector& other):
	x(other.x), 
	y(other.y)
{}

Vector& Vector::operator=(const Vector& other) {
	x = other.x;
	y = other.y;
	return *this;
}

Vector::Vector(float x_, float y_): 
	x(x_),
	y(y_)
{}

Vector Vector::operator+(const Vector& other) const { return Vector(x + other.x, y + other.y); }
Vector Vector::operator-(const Vector& other) const { return Vector(x - other.x, y - other.y); }
Vector Vector::operator*(const float& factor) const { return Vector(x * factor, y * factor); }
Vector Vector::operator/(const float& divisor) const { return Vector(x / divisor, y / divisor); }

Vector& Vector::operator+=(const Vector& other) { *this = (*this) + other; return *this; }
Vector& Vector::operator-=(const Vector& other) { *this = (*this) - other; return *this; }
Vector& Vector::operator*=(const float& factor) { *this = (*this) * factor; return *this; }

bool Vector::operator==(const Vector& other) const { return (x == other.x && y == other.y); }

Vector Vector::operator-() const { return Vector(-x, -y); }

std::ostream& operator<<(std::ostream& os, const Vector& other) { return os << "(" << other.x << ", " << other.y << ")"; }

namespace math {
	float length(const Vector& vector) {
		return std::hypot(vector.x, vector.y);
	}
	float distance(const Vector& lhs, const Vector& rhs) {
		return std::hypot(lhs.x - rhs.x, lhs.y - rhs.y);
	}
	Vector normalize(const Vector& vector) {
		return vector / length(vector);
	}

	Vector rotate(const Vector& vector, float angle) {
		// A little linear algebra!!
		return Vector(vector.x * cos_degree(angle) - vector.y * sin_degree(angle),
		              vector.x * sin_degree(angle) + vector.y * cos_degree(angle));
	}
} // namespace math
