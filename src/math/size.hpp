#ifndef HEADER_ASAED_MATH_SIZE_HPP
#define HEADER_ASAED_MATH_SIZE_HPP

#include <iosfwd>

class Sizef;

class Size final {
public :
	int width;
	int height;

public :
	Size();
	Size(int width_, int height_);

	Size(const Size& rhs) = default;
	Size& operator=(const Size& rhs) = default;

	explicit Size(const Sizef& rhs);

	Size& operator*=(int factor);
	Size& operator/=(int divisor);
	Size& operator+=(const Size& rhs);
	Size& operator-=(const Size& rhs);

	friend Size operator*(const Size& lhs, int factor);
	friend Size operator*(int factor, const Size& rhs);
	friend Size operator/(const Size& lhs, int divisor);
	friend Size operator+(const Size& lhs, const Size& rhs);
	friend Size operator-(const Size& lhs, const Size& rhs);
	friend bool operator==(const Size& lhs, const Size& rhs); 
	friend bool operator!=(const Size& lhs, const Size& rhs);

	bool is_valid() const;
};

std::ostream& operator<<(std::ostream& s, const Size& size);

#endif