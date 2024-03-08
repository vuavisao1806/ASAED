#ifndef HEADER_ASAED_MATH_RECT_HPP
#define HEADER_ASAED_MATH_RECT_HPP

#include <iosfwd>

#include <algorithm>
#include <tuple>
#include <SDL.h>

#include "math/size.hpp"

class Rectf;

class Rect final {
public:
	int left;
	int top;
	int right;
	int bottom;

public:
	static Rect from_center(int center_x, int center_y, int width, int height);

public:
	Rect();
	Rect(int left_, int top_, int right_, int bottom_);

	Rect(const Rect& rhs) = default;
	Rect& operator=(const Rect& rhs) = default;

	Rect(int left_, int top_, const Size& size);
	Rect(const SDL_Rect& rect);

	explicit Rect(const Rectf& other);

	bool operator==(const Rect& other) const;
	bool contains(int x, int y) const;
	bool contains(const Rect& other) const;
	
	const int& get_left() const;
	const int& get_right() const;
	const int& get_top() const;
	const int& get_bottom() const;

	int get_width() const;
	int get_height() const;
	Size get_size() const;
	int get_area() const;

	bool empty() const;
	bool valid() const;
	
	Rect normalized() const;
	Rect moved(int x, int y) const;
	Rect grown(int border) const;

	Rectf to_rectf() const;
	SDL_Rect to_sdl() const;

	bool operator<(const Rect& other) const;
};

std::ostream& operator<<(std::ostream& out, const Rect& rect);

#endif