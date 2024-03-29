#ifndef HEADER_ASAED_COLLISION_COLLISION_OBJECT_HPP
#define HEADER_ASAED_COLLISION_COLLISION_OBJECT_HPP

#include <stdint.h>

#include "collision/collision_group.hpp"
#include "collision/collision_hit.hpp"
#include "math/rectf.hpp"

/** to specify the information on every object to check collision and move in the screen */

class CollisionObject {
	friend class CollisionSystem;
public:
	/** the bounding box of the object (as used for collision detection) */
	Rectf m_bounding_box;

	/** the collision group */
	CollisionGroup m_group;

private:
	/** the movement that will happen till next frame (as used for collision checking) */
	Vector m_movement;

	/** give the last destination during collision detection */
	Rectf m_dest;

public:
	CollisionObject();
	CollisionObject(CollisionGroup& group);

private:
	CollisionObject(const CollisionObject&) = delete;
	CollisionObject& operator=(const CollisionObject&) = delete;

public:
	/** this function is called when the object collided with something solid (wall and obstacle) */
	virtual void collision_solid(const CollisionHit& hit) = 0;
	
	/** this function is called when the object collided with any other object (ordinary) */
	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) = 0;

	/** this function is called when the object collided with tiles with special attributes */
	virtual void collision_tile(uint32_t tile_attributes) = 0;

public:
	virtual bool is_valid() const = 0;

public:
	const Rectf& get_bounding_box() const;
	
	const Vector& get_movement() const;
	void set_movement(const Vector& movement);

	Vector get_pos() const;
	/** Be careful!! When you move the destination, no collision detection happens */
	void set_pos(const Vector& pos); 

	/** Be careful!! When you set the size, no collision detection happens */
	void set_size(float width, float height);

	CollisionGroup get_group() const;
};

#endif