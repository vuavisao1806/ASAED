#include "collision/collision_system.hpp"

#include "asaed/constants.hpp"
#include "asaed/room.hpp"
#include "collision/collision_object.hpp"
#include "object/tile.hpp"
#include "object/tile_map.hpp"
#include "math/rectf.hpp"
#include "object/moving_object.hpp"
#include "object/tile.hpp"
#include "video/drawing_context.hpp"
#include "video/color.hpp"
#include "video/layer.hpp"

#include <iostream> // test

namespace {
	// arbitrary (pixel / frame)
	const float MAX_SPEED = 8.0f; 
}

CollisionSystem::CollisionSystem(Room& room) :
	m_room(room),
	m_objects()
{}

void CollisionSystem::add(CollisionObject* object) { m_objects.push_back(object); }
void CollisionSystem::remove(CollisionObject* object) {
	// O(n) ?? But don't worry, it's enough small and good !!
	m_objects.erase(std::find(m_objects.begin(), m_objects.end(), object));
}

void CollisionSystem::draw_debug(DrawingContext& context) {
	for (auto& object : m_objects) {
		Color color;
		switch (object->get_group()) {
			case COLLISION_MOVING:
				color = Color::BLUE;
				break;
			
			default:
				color = Color::GREEN;
				break;
		}
		const Rectf& rect = object->get_bounding_box();
		context.get_canvas().draw_filled_rect(rect, color, LAYER_FOREGROUND + 10);
	}
}

namespace {
	collision::Constraints check_collision(const Vector& object_movement, const Rectf& moving_object_rect, const Rectf& other_object_rect, CollisionObject* moving_object = nullptr) {
		collision::Constraints constraints;

		// slightly growing the static object (actually only tilemap) to collision detection when extremely near
		const Rectf grown_other_object_rect = other_object_rect.grown(EPSILON);

		if (!moving_object_rect.overlaps(grown_other_object_rect)) {
			return constraints;
		}

		float intersect_left = moving_object_rect.get_right() - grown_other_object_rect.get_left();
		float intersect_top = moving_object_rect.get_bottom() - grown_other_object_rect.get_top();
		float intersect_right = grown_other_object_rect.get_right() - moving_object_rect.get_left();
		float intersect_bottom = grown_other_object_rect.get_bottom() - moving_object_rect.get_top();

		bool shift_out = false;
	
		if (moving_object) {
			if (std::abs(object_movement.x) < std::abs(object_movement.y)) {
				if (intersect_left < SHIFT_DELTA) {
					constraints.constrain_right(grown_other_object_rect.get_left());
					shift_out = true;
				}
				else if (intersect_right < SHIFT_DELTA) {
					constraints.constrain_left(grown_other_object_rect.get_right());
					shift_out = true;
				}
			}
			else {
				if (intersect_top < SHIFT_DELTA) {
					constraints.constrain_bottom(grown_other_object_rect.get_top());
					shift_out = true;
				}
				else if (intersect_bottom < SHIFT_DELTA) {
					constraints.constrain_top(grown_other_object_rect.get_bottom());
					shift_out = true;
				}
			}
		}

		if (!shift_out) {
			const float horizontal_intersect = std::min(intersect_left, intersect_right);
			const float vertical_intersect = std::min(intersect_top, intersect_bottom);
			
			if (horizontal_intersect < vertical_intersect) {
				if (intersect_left < intersect_right) {
					constraints.constrain_right(grown_other_object_rect.get_left());
					constraints.hit.right = true;
				}
				else {
					// std::cout << moving_object_rect.get_right() << '\n';
					// std::cout << "collision " << grown_other_object_rect.get_right() << '\n';
					constraints.constrain_left(grown_other_object_rect.get_right());
					constraints.hit.left = true;
				}
			}
			else {
				if (intersect_top < intersect_bottom) {
					constraints.constrain_bottom(grown_other_object_rect.get_top());
					constraints.hit.bottom = true;
				}
				else {
					constraints.constrain_top(grown_other_object_rect.get_bottom());
					constraints.hit.top = true;
				}
			}
		}
		return constraints;
	}

	/** filled information of collision between 2 rectangles into hit and normal */
	void get_hit_normal(const Rectf& r1, const Rectf& r2, CollisionHit& hit, Vector& normal) {
		float intersect_left = r1.get_right() - r2.get_left();
		float intersect_top = r1.get_bottom() - r2.get_top();
		float intersect_right = r2.get_right() - r1.get_left();
		float intersect_bottom = r2.get_bottom() - r1.get_top();

		const float horizontal_intersect = std::min(intersect_left, intersect_right);
		const float vertical_intersect = std::min(intersect_top, intersect_bottom);

		if (horizontal_intersect < vertical_intersect) {
			if (intersect_left < intersect_right) {
				hit.right = true;
				normal.x = horizontal_intersect;
			}
			else {
				hit.left = true;
				normal.x = -horizontal_intersect;
			}
		}
		else {
			if (intersect_top < intersect_bottom) {
				hit.bottom = true;
				normal.y = vertical_intersect;
			}
			else {
				hit.top = true;
				normal.y = -vertical_intersect;
			}
		}
	}
}

void CollisionSystem::update() {
	// calculate the destination positions of the objects
	// guarantee all velocity can't be bigger MAX_SPEED
	for (const auto& object : m_objects) {
		const auto& mov = object->get_movement();
		if (math::length(mov) > MAX_SPEED) {
			object->set_movement(math::normalize(mov) * MAX_SPEED);
		}
		object->m_dest = object->m_bounding_box;
		object->m_dest.move(object->get_movement());
	}


	// STEP1: Check collision COLLISION_MOVING with tilemap
	for (const auto& object : m_objects) {
		if (object->get_group() != COLLISION_MOVING || !object->is_valid()) continue;
		collision_static_constraints(*object);
	}

	// STEP2: COLLISION_MOVING with tile attributes
	for (const auto& object : m_objects) {
		if (object->get_group() != COLLISION_MOVING || !object->is_valid()) continue;
		uint32_t tile_attributes = collision_tile_attributes(object->m_dest);
		if (tile_attributes >= Tile::FIRST_INTERESTING_FLAG) {
			object->collision_tile(tile_attributes);
		}
	}

	// STEP3 : COLLISION_MOVING with COLLISION_MOVING
	// o(n^2) but everything is okay

	for (auto it = m_objects.begin(); it != m_objects.end(); ++ it) {
		auto object1 = *it;
		if (object1->get_group() != COLLISION_MOVING || !object1->is_valid()) continue;
		for (auto jt = it + 1; jt != m_objects.end(); ++ jt) {
			auto object2 = *jt;
			if (object2->get_group() != COLLISION_MOVING || !object2->is_valid()) continue;
			collision_object(*object1, *object2);
		}
	}

	for (auto* object : m_objects) {
		object->m_bounding_box = object->m_dest;
		object->set_movement(Vector(0.0f, 0.0f));
	}
}

void CollisionSystem::collision_static(collision::Constraints* constraints,
                                       const Vector& movement, const Rectf& dest,
                                       CollisionObject& object) const {
	collision_tilemap(constraints, movement, dest, object);
}

void CollisionSystem::collision_tilemap(collision::Constraints* constraints,
                                        const Vector& movement, const Rectf& dest,
                                        CollisionObject& object) const {
	for (auto solid_tilemap : m_room.get_solid_tilemaps()) {
		const Rect test_tiles = solid_tilemap->get_tiles_overlapping(dest);
		for (int x = test_tiles.get_left(); x < test_tiles.get_right(); ++ x) {
			for (int y = test_tiles.get_top(); y < test_tiles.get_bottom(); ++ y) {
				const Tile& tile = solid_tilemap->get_tile(x, y);
				if (!tile.is_solid()) {
					continue;
				}
				Rectf tile_bounding_box = solid_tilemap->get_tile_bounding_box(x, y);
				collision::Constraints new_constraints = check_collision(movement, dest, tile_bounding_box, &object);
				constraints->merge_constrains(new_constraints);
			}
		}
	}
}

uint32_t CollisionSystem::collision_tile_attributes(const Rectf& dest) const {
	uint32_t result = 0;
	for (auto solid_tilemap : m_room.get_solid_tilemaps()) {
		const Rect test_tiles = solid_tilemap->get_tiles_overlapping(dest);
		for (int x = test_tiles.get_left(); x < test_tiles.get_right(); ++ x) {
			for (int y = test_tiles.get_top(); y < test_tiles.get_bottom(); ++ y) {
				const Tile& tile = solid_tilemap->get_tile(x, y);
				result |= tile.get_attributes();
			}
		}
	}
	return result;
}

void CollisionSystem::collision_static_constraints(CollisionObject& object) const {
	collision::Constraints constraints;
	const Vector movement = object.m_movement;
	Rectf& dest = object.m_dest;

	for (int i = 0; i < 2; ++ i) {
		collision_static(&constraints, Vector(0.0f, movement.y), dest, object);
		if (!constraints.has_constrains()) {
			break;
		}
		// std::cout << "bottom: " << constraints.has_constrains_bottom() << '\n';
		// std::cout << "top: " << constraints.has_constrains_top() << '\n';
		if (constraints.has_constrains_bottom()) {
			dest.set_bottom(constraints.get_position_bottom() - EPSILON);
			dest.set_top(dest.get_bottom() - object.get_bounding_box().get_height());
		}
		else if (constraints.has_constrains_top()) {
			dest.set_top(constraints.get_position_top() + EPSILON);
			dest.set_bottom(dest.get_top() + object.get_bounding_box().get_height());
		}
	}

	if (constraints.has_constrains()) {
		if (constraints.hit.top || constraints.hit.bottom) {
			constraints.hit.left = false;
			constraints.hit.right = false;
			object.collision_solid(constraints.hit);
		}
	}

	constraints = collision::Constraints();

	for (int i = 0; i < 2; ++ i) {
		collision_static(&constraints, movement, dest, object);
		if (!constraints.has_constrains()) {
			break;
		}
		// std::cout << "right: " << constraints.has_constrains_right() << '\n';
		// std::cout << "left: " << constraints.has_constrains_left() << '\n';
		if (constraints.has_constrains_right()) {
			dest.set_right(constraints.get_position_right() - EPSILON);
			dest.set_left(dest.get_right() - object.get_bounding_box().get_width());
		}
		else if(constraints.has_constrains_left()) {
			dest.set_left(constraints.get_position_left() + EPSILON);
			dest.set_right(dest.get_left() + object.get_bounding_box().get_width());
		}
	}

	if (constraints.has_constrains()) {
		if (constraints.hit.top || constraints.hit.bottom || constraints.hit.left || constraints.hit.right) {
			object.collision_solid(constraints.hit);
		}
	}
}

void CollisionSystem::collision_object(CollisionObject& object1, CollisionObject& object2) const {
	for (int i = 0; i < 2; ++ i) {
		const Rectf& r1 = object1.m_dest;
		const Rectf& r2 = object2.m_dest;

		if (!r1.overlaps(r2)) return;
		CollisionHit hit;
		Vector normal(0.0f, 0.0f);
		get_hit_normal(r1, r2, hit, normal);

		HitResponse response1 = object1.collision(object2, hit);
		std::swap(hit.left, hit.right);
		std::swap(hit.top, hit.bottom);
		HitResponse response2 = object2.collision(object1, hit);

		if (response1 == CONTINUE && response2 == CONTINUE) {
			normal *= (0.5f + EPSILON);
			object1.m_dest.move(-normal);
			object2.m_dest.move(normal);
		}
		else if (response1 == CONTINUE && response2 == FORCE_MOVE) {
			normal *= (1.0f + EPSILON);
			object1.m_dest.move(-normal);
		}
		else if(response1 == FORCE_MOVE && response2 == CONTINUE) {
			normal *= (1.0f + EPSILON);
			object2.m_dest.move(-normal);
		}
	}
}