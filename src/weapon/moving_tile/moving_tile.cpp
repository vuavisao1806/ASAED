#include "weapon/moving_tile/moving_tile.hpp"

#include "asaed/room.hpp"

MovingTile::MovingTile(const std::string& filename) :
	MovingSprite(Vector(0.0f, 0.0f), filename, LAYER_MOVING_TILE),
	m_hurt_attributes(),
	m_physic(),
	m_damage(),
	m_ratio_crit()
{}

void MovingTile::collision_solid(const CollisionHit& hit) { remove_me(); }
HitResponse MovingTile::collision(CollisionObject& /* other */, const CollisionHit& /* hit */) { return ABORT_MOVE; }
void MovingTile::collision_tile(uint32_t /* tile_attributes */) {}
	
void MovingTile::update(float dt_sec) {
	if (!Room::get().inside(get_bounding_box())) {
		remove_me();
	}
	set_movement(m_physic.get_movement(dt_sec));
}

std::string MovingTile::class_name() { return "movingtile"; }
std::string MovingTile::get_class_name() const { return class_name(); };

uint32_t MovingTile::get_hurt_attributes() const { return m_hurt_attributes; }
void MovingTile::set_hurt_attributes(uint32_t hurt_attributes) { m_hurt_attributes = hurt_attributes; }

int MovingTile::get_damage() const { return m_damage; }
float MovingTile::get_ratio_crit() const { return m_ratio_crit; }