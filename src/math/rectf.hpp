#ifndef HEADER_ASAED_MATH_RECTF_HPP
#define HEADER_ASAED_MATH_RECTF_HPP

#include <assert.h>
#include <iosfwd>

#include <SDL.h>

#include "math/sizef.hpp"
#include "math/vector.hpp"

class Rect;

class Rectf final {
private :
	Vector m_p1; // Point upper left
	Sizef m_size;

public : 
	static Rectf from_center(const Vector& center, const Sizef& size);

public :
	Rectf();

	Rectf(const Rectf& rhs) = default;
	Rectf& operator=(const Rectf& rhs) = default;

	Rectf(const Vector& np1, const Vector& np2);
	Rectf(float x1, float y1, float x2, float y2);
	Rectf(const Vector& p1, const Sizef& size);
	Rectf(const SDL_FRect& rect);

	Rectf(const Rect& rect);

	bool operator==(const Rectf& other) const;

	float get_left() const;
	float get_right() const;
	float get_top() const;
	float get_bottom() const;
	float get_width() const;
	float get_height() const;

	void set_left(float v);
	void set_right(float v);
	void set_top(float v);
	void set_bottom(float v);
	void set_width(float width);
	void set_height(float height);
	void set_size(float width, float height);
	void set_pos(const Vector& v);

	Vector get_middle() const;
	
	Sizef get_size() const;
	
	bool empty() const;

	void move(const Vector& v);
	Rectf moved(const Vector& v) const;
	
	bool contains(const Vector& v) const;
	bool overlaps(const Rectf& other) const;

	float distance(const Vector& other) const;
	float distance(const Rectf& other) const;

	Rectf grown(float border);

	Vector p1() const;
	Vector p2() const;

	void set_p1(const Vector& p);
	void set_p2(const Vector& p);

	Rect to_rect();	
	SDL_FRect to_sdl() const;
};

std::ostream& operator<<(std::ostream& out, const Rectf& rect);

#endif