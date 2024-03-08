#include "math/rect.hpp"

#include <ostream>

#include "math/rectf.hpp"

Rect Rect::from_center(int center_x, int center_y, int width, int height) {
	return Rect(center_x - width / 2,
	            center_y - height / 2,
	            center_x + width / 2,
	            center_y + height / 2);
}

Rect::Rect():
	left(0),
	top(0),
	right(0),
	bottom(0)
{}

Rect::Rect(int left_, int top_, int right_, int bottom_):
	left(left_),
	top(top_),
	right(right_),
	bottom(bottom_)
{}

Rect::Rect(int left_, int top_, const Size& size):
	left(left_),
	top(top_),
	right(left_ + size.width),
	bottom(top + size.height)
{}

Rect::Rect(const SDL_Rect& rect):
	left(rect.x),
	top(rect.y),
	right(rect.x + rect.w),
	bottom(rect.y + rect.h)
{}

Rect::Rect(const Rectf& other):
	left(static_cast<int>(other.get_left())),
	top(static_cast<int>(other.get_top())),
	right(static_cast<int>(other.get_right())),
	bottom(static_cast<int>(other.get_bottom()))
{}

bool Rect::operator==(const Rect& other) const {
	return (left == other.left && top == other.top && right == other.right && bottom == other.bottom);
}

bool Rect::contains(int x, int y) const {
	return (left <= x && x < right && top <= y && y < bottom);
}

bool Rect::contains(const Rect& other) const {
	return (left <= other.left && other.right <= right && top <= other.top && other.bottom <= bottom);
}


const int& Rect::get_left() const { return left; }
const int& Rect::get_right() const { return right; }
const int& Rect::get_top() const { return top; }
const int& Rect::get_bottom() const { return bottom; }


int Rect::get_width() const { return right - left; }
int Rect::get_height() const { return bottom - top; }
Size Rect::get_size() const { return Size(right - left, bottom - top); }
int Rect::get_area() const { return get_width() * get_height(); }


bool Rect::empty() const { return (get_width() <= 0 || get_height() <= 0); }
bool Rect::valid() const { return (left <= right && top <= bottom); }


Rect Rect::normalized() const {
	return Rect(std::min(left, right),
	            std::min(top, bottom),
	            std::max(left, right),
	            std::max(top, bottom));
}
Rect Rect::moved(int x, int y) const {
	return Rect(left + x,
	            top + y,
	            right + x,
	            bottom + y);
}
Rect Rect::grown(int border) const {
	return Rect(left - border,
	            top - border,
	            right + border,
	            bottom + border);
}

Rectf Rect::to_rectf() const {
	return {static_cast<float>(left), static_cast<float>(top), static_cast<float>(right), static_cast<float>(bottom)};
}	
SDL_Rect Rect::to_sdl() const {
	return {left, top, get_width(), get_height()};
}


bool Rect::operator<(const Rect& other) const {
	return std::tie(left, top, right, bottom) < std::tie(other.left, other.top, other.right, other.bottom);
}

std::ostream& operator<<(std::ostream& out, const Rect& rect) {
	out << "Rect("
	    << rect.left << ", " << rect.top << ", "
	    << rect.right << ", " << rect.bottom
	    << ")";
	return out;
}