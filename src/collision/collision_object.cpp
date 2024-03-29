#include "collision/collision_object.hpp"

CollisionObject::CollisionObject() :
	m_bounding_box(),
	m_group(COLLISION_DISABLED),
	m_movement(),
	m_dest()
{}

CollisionObject::CollisionObject(CollisionGroup& group) :
	m_bounding_box(),
	m_group(group),
	m_movement(),
	m_dest()
{}

const Rectf& CollisionObject::get_bounding_box() const { return m_bounding_box; }

const Vector& CollisionObject::get_movement() const { return m_movement; }
void CollisionObject::set_movement(const Vector& movement) { m_movement = movement; }

Vector CollisionObject::get_pos() const { return m_bounding_box.p1(); }
void CollisionObject::set_pos(const Vector& pos) {
	m_dest.move(pos - get_pos());
	m_bounding_box.set_pos(pos);
} 

void CollisionObject::set_size(float width, float height) {
	m_bounding_box.set_size(width, height);
	m_dest.set_size(width, height);
}

CollisionGroup CollisionObject::get_group() const { return m_group; }