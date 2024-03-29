#include "object/moving_object.hpp"


MovingObject::MovingObject() {
	set_group(COLLISION_MOVING);
}

MovingObject::~MovingObject() 
{}

void MovingObject::collision_solid(const CollisionHit& /* hit */) {}
HitResponse MovingObject::collision(CollisionObject& /* other */, const CollisionHit& /* hit */) { return ABORT_MOVE; }
void MovingObject::collision_tile(uint32_t /* tile_attributes */ ) {}

bool MovingObject::is_valid() const { return GameObject::is_valid(); }

std::string MovingObject::class_name() { return "moving-object"; }
std::string MovingObject::get_class_name() const { return class_name(); }

void MovingObject::set_group(CollisionGroup collision_group) {
	m_group = collision_group;
}