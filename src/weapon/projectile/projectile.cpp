#include "weapon/projectile/projectile.hpp"

#include "asaed/room.hpp"
#include "badguy/badguy.hpp"
#include "object/player.hpp"
#include "weapon/hurt.hpp"

Projectile::Projectile(const std::string& filename) :
	MovingSprite(Vector(0.0f, 0.0f), filename, LAYER_MOVING_TILE),
	m_hurt_attributes(),
	m_physic(),
	m_damage(),
	m_ratio_crit()
{}

void Projectile::collision_solid(const CollisionHit& /* hit */) { remove_me(); }

HitResponse Projectile::collision(CollisionObject& other, const CollisionHit& /* hit */) {
	if (dynamic_cast<Player*>(&other)) {
		if (!(m_hurt_attributes & HURT_PLAYER)) {
			return ABORT_MOVE;
		}
		remove_me();
		return FORCE_MOVE;
	}

	if (dynamic_cast<BadGuy*>(&other)) {
		if (!(m_hurt_attributes & HURT_BADGUY)) {
				return ABORT_MOVE;
		}
		remove_me();
		return FORCE_MOVE;
	}

	if (dynamic_cast<Projectile*>(&other)) {
		return ABORT_MOVE;
	}

	return CONTINUE;
}

void Projectile::collision_tile(uint32_t /* tile_attributes */) {}
	
void Projectile::update(float dt_sec) {
	if (!Room::get().inside(get_bounding_box())) {
		remove_me();
	}
	set_movement(m_physic.get_movement(dt_sec));
}

std::string Projectile::class_name() { return "projectile"; }
std::string Projectile::get_class_name() const { return class_name(); };

uint32_t Projectile::get_hurt_attributes() const { return m_hurt_attributes; }
void Projectile::set_hurt_attributes(uint32_t hurt_attributes) { m_hurt_attributes = hurt_attributes; }

int Projectile::get_damage() const { return m_damage; }
float Projectile::get_ratio_crit() const { return m_ratio_crit; }

Vector Projectile::get_velocity() const { return m_physic.get_velocity(); }
void Projectile::set_velocity(const Vector& velocity) { m_physic.set_velocity(velocity); }

Physic& Projectile::get_physic() { return m_physic; }