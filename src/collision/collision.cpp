
#include "collision/collision.hpp"

#include <algorithm>
#include <limits>

#include "math/rectf.hpp"

namespace {
	constexpr float INFINITY = std::numeric_limits<float>::has_infinity ? 
	                           std::numeric_limits<float>::infinity() :
	                           std::numeric_limits<float>::max();
}

namespace collision {
	Constraints::Constraints() :
		hit(),
		position_left(-INFINITY),
		position_top(-INFINITY),
		position_right(INFINITY),
		position_bottom(INFINITY)
	{}

	bool Constraints::has_constrains() const {
		return has_constrains_left()  ||
		       has_constrains_top()   ||
		       has_constrains_right() ||
		       has_constrains_bottom();
	}

	bool Constraints::has_constrains_left() const { return position_left > -INFINITY; }
	bool Constraints::has_constrains_top() const { return position_top > -INFINITY; }
	bool Constraints::has_constrains_right() const { return position_right < INFINITY; }
	bool Constraints::has_constrains_bottom() const { return position_bottom < INFINITY; }

	float Constraints::get_position_left() const { return position_left; }
	float Constraints::get_position_top() const { return position_top; }
	float Constraints::get_position_right() const { return position_right; }
	float Constraints::get_position_bottom() const { return position_bottom; }

	void Constraints::constrain_left(float position) { position_left = std::max(position_left, position); }
	void Constraints::constrain_top(float position) { position_top = std::max(position_top, position); }
	void Constraints::constrain_right(float position) { position_right = std::min(position_right, position);}
	void Constraints::constrain_bottom(float position) { position_bottom = std::min(position_bottom, position); }
	
	void Constraints::merge_constrains(const Constraints& other) {
		constrain_left(other.get_position_left());
		constrain_top(other.get_position_top());
		constrain_right(other.get_position_right());
		constrain_bottom(other.get_position_bottom());

		hit.left |= other.hit.left;
		hit.top |= other.hit.top;
		hit.right |= other.hit.right;
		hit.bottom |= other.hit.bottom;
	}

		
	bool intersects(const Rectf& lhs, const Rectf& rhs) {
		if (lhs.get_right() < rhs.get_left() || rhs.get_right() < lhs.get_left()) return false;
		if (lhs.get_bottom() < rhs.get_top() || rhs.get_bottom() < lhs.get_top()) return false;
		return true;
	}
}