#ifndef HEADER_ASAED_COLLISION_COLLISION_SYSTEM_HPP
#define HEADER_ASAED_COLLISION_COLLISION_SYSTEM_HPP

#include <stdint.h>
#include <vector>

#include "collision/collision.hpp"
#include "math/vector.hpp"

class CollisionObject;
class DrawingContext;
class Rectf;
class Room;

class CollisionSystem final {
	Room& m_room;
	std::vector<CollisionObject*> m_objects;

private:
	CollisionSystem(const CollisionSystem&) = delete;
	CollisionSystem& operator=(const CollisionSystem&) = delete;

public:
	CollisionSystem(Room& room);

public:
	void add(CollisionObject* object);
	void remove(CollisionObject* object);

	/** I love it! Draw collision shapes for debugging */
	void draw_debug(DrawingContext& context);

	/** Most important!! Check for all possible collisions and calls the suitable collision_handler to handle this */
	void update();

private:
	void collision_static(collision::Constraints* constraints,
	                      const Vector& movement, const Rectf& dest,
	                      CollisionObject& object) const;

	void collision_tilemap(collision::Constraints* constraints,
	                       const Vector& movement, const Rectf& dest,
	                       CollisionObject& object) const;

	uint32_t collision_tile_attributes(const Rectf& dest) const;

	void collision_static_constraints(CollisionObject& object) const;

	void collision_object(CollisionObject& object1, CollisionObject& object2) const;
};

#endif