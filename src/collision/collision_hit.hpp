#ifndef HEADER_ASAED_COLLISION_COLLISION_HIT_HPP
#define HEADER_ASAED_COLLISION_COLLISION_HIT_HPP

/** used to return value for identifying type collision should be handled */

enum HitResponse {
	ABORT_MOVE = 0,
	// moving the object out of collision
	CONTINUE,
	// treat object as having inf mass
	// force pushing every other (CONTINUE) object out of the way
	FORCE_MOVE
};

/** CollisionHit to collect information about the collision */

class CollisionHit final {
public:
	bool left;
	bool top;
	bool right;
	bool bottom;

	CollisionHit() :
		left(false),
		top(false),
		right(false),
		bottom(false)
	{}
};

#endif