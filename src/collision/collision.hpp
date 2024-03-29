#ifndef HEADER_ASAED_COLLISION_COLLISION_HPP
#define HEADER_ASAED_COLLISION_COLLISION_HPP

#include "collision/collision_hit.hpp"

class Rectf;

namespace collision {
	/** to store specific information about the collision */
	class Constraints final {
	public:
		CollisionHit hit;
		
	private:
		float position_left;
		float position_top;
		float position_right;
		float position_bottom;

	public:
		Constraints();

	public:
		bool has_constrains() const;
		bool has_constrains_left() const;
		bool has_constrains_top() const;
		bool has_constrains_right() const;
		bool has_constrains_bottom() const;

		float get_position_left() const;
		float get_position_top() const;
		float get_position_right() const;
		float get_position_bottom() const;

		void constrain_left(float position);
		void constrain_top(float position);
		void constrain_right(float position);
		void constrain_bottom(float position);
	
		void merge_constrains(const Constraints& other);
	};

	bool intersects(const Rectf& lhs, const Rectf& rhs);
} // namespace collision

#endif