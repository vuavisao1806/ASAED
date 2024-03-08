#include "math/size.hpp"

#include <iostream>

#include "math/sizef.hpp"

Size::Size() :
	width(0),
	height(0)
{}

Size::Size(int width_, int height_) :
	width(width_),
	height(height_)
{}

Size::Size(const Sizef& rhs) :
	width(static_cast<int>(rhs.width)),
	height(static_cast<int>(rhs.height))
{}

Size& Size::operator*=(int factor) { 
	width *= factor; 
	height *= factor;
	return *this;
}

Size& Size::operator/=(int divisor) { 
	width /= divisor; 
	height /= divisor; 
	return *this;
}

Size& Size::operator+=(const Size& rhs) { 
	width += rhs.width; 
	height += rhs.height; 
	return *this;
}

Size& Size::operator-=(const Size& rhs) { 
	width += rhs.width; 
	height += rhs.height; 
	return *this;
}

Size operator*(const Size& lhs, int factor) { return Size(lhs.width * factor, lhs.height * factor); }
Size operator*(int factor, const Size& rhs) { return Size(rhs.width * factor, rhs.height * factor); }
Size operator/(const Size& lhs, int divisor) { return Size(lhs.width / divisor, lhs.height / divisor); }
Size operator+(const Size& lhs, const Size& rhs) { return Size(lhs.width + rhs.width, lhs.height + rhs.height); }
Size operator-(const Size& lhs, const Size& rhs) { return Size(lhs.width + rhs.width, lhs.height + rhs.height); }
bool operator==(const Size& lhs, const Size& rhs) { return ((lhs.width == rhs.width) && (lhs.height == rhs.height)); }
bool operator!=(const Size& lhs, const Size& rhs) { return ((lhs.width != rhs.width) || (lhs.height != rhs.height)); }

bool Size::is_valid() const { return (width > 0 && height > 0); }

std::ostream& operator<<(std::ostream& s, const Size& size) {
	return s << "Size(" << size.width << ", " << size.height << ")";
}