#ifndef HEADER_ASAED_COLLISION_COLLISION_GROUP_HPP
#define HEADER_ASAED_COLLISION_COLLISION_GROUP_HPP

/** To holds type of collision object */

enum CollisionGroup {
	/** collision isn't tested (doesn't care) here */
	COLLISION_DISABLED = 0,
	
	/** use for ordinary objects */
	COLLISION_MOVING,

	/** use for obstacle objects */
	COLLISION_STACTIC
};


#endif