#include "collision/collision_system.hpp"

#include <vector>
#include <cmath>

#include "asaed/constants.hpp"
#include "asaed/room.hpp"
#include "badguy/badguy.hpp"
#include "collision/collision_object.hpp"
#include "object/tile.hpp"
#include "object/tile_map.hpp"
#include "math/rectf.hpp"
#include "object/moving_object.hpp"
#include "object/player.hpp"
#include "object/tile.hpp"
#include "video/drawing_context.hpp"
#include "video/color.hpp"
#include "video/layer.hpp"

namespace {
	// arbitrary (pixel / frame)
	const float MAX_SPEED = 12.0f;
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
	
		if (moving_object && (dynamic_cast<Player*>(moving_object) || dynamic_cast<BadGuy*>(moving_object))) {
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

	struct VectorInt {
		int x, y;
		VectorInt() = delete;
		VectorInt(int x_, int y_) :
			x(x_),
			y(y_)
		{}

		Vector operator*(float factor) const { return Vector(static_cast<float>(x) * factor, static_cast<float>(y) * factor); }
	};

	int iPart(float x) { return static_cast<int>(std::floor(x)); }
	float fPart(float x) { return x - std::floor(x); }
	float rfPart(float x) { return 1.0f - fPart(x); }

	void drawLine(int x, int y, float ratio, std::vector<VectorInt>& line) {
		if (ratio <= 0.0f) return;
		line.push_back(VectorInt(x, y));
	}

	// https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm for more details
	std::vector<VectorInt> Xiaolin_Wu_line(float x1, float y1, float x2, float y2) {
		const bool steep = std::fabs(y2 - y1) > std::fabs(x2 - x1);

		if (steep) {
			std::swap(x1, y1);
			std::swap(x2, y2);
		}
		if (x1 > x2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		const float dx = x2 - x1;
		const float dy = y2 - y1;
		const float slope = (dx == 0.0f ? 1.0f : dy / dx);

		std::vector<VectorInt> line;

		int x_start = static_cast<int>(std::round(x1));
		int x_end = static_cast<int>(std::round(x2));
		float intersectY = y1;

		if (steep) {
			while (x_start <= x_end) {
				drawLine(iPart(intersectY), x_start, rfPart(intersectY), line);
				drawLine(iPart(intersectY) - 1, x_start, fPart(intersectY), line);
				++ x_start;
				intersectY += slope;
			}
		}
		else {
			while (x_start <= x_end) {
				drawLine(x_start, iPart(intersectY), rfPart(intersectY), line);
				drawLine(x_start, iPart(intersectY) - 1, fPart(intersectY), line);
				++ x_start;
				intersectY += slope;
			}
		}

		return line;
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


bool CollisionSystem::is_free_of_tiles(const Rectf& rect) const {
	for (const auto& solid : m_room.get_solid_tilemaps()) {
		const Rect test_tiles = solid->get_tiles_overlapping(rect);
		for (int x = test_tiles.get_left(); x < test_tiles.get_right(); ++ x) {
			for (int y = test_tiles.get_top(); y < test_tiles.get_bottom(); ++ y) {
				const auto& Tile = solid->get_tile(x, y);
				if (!Tile.is_solid()) {
					continue;
				}
				return false;
			}
		}
	}
	return true;
}

bool CollisionSystem::free_light_of_sight(const Vector& line_start, const Vector& line_end) const {
	// fix fixel duplicate in 1 BLOCK
	const float x1 = line_start.x / BLOCK_SIZE;
	const float y1 = line_start.y / BLOCK_SIZE;
	const float x2 = line_end.x / BLOCK_SIZE;
	const float y2 = line_end.y / BLOCK_SIZE;

	std::vector<VectorInt> test_line = Xiaolin_Wu_line(x1, y1, x2, y2);
	for (const auto& solid : m_room.get_solid_tilemaps()) {
		for (const auto& point : test_line) {
			Vector test_point = point * BLOCK_SIZE;
			if (solid->is_outside_bounds(test_point)) {
				continue;
			}
			const Tile& tile = solid->get_tile_at(test_point);
			if (tile.is_solid()) {
				return false;
			}
		}
	}
	return true;
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
	for (int i = 0; i < 1; ++ i) {
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
			normal *= (0.25f + EPSILON);
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