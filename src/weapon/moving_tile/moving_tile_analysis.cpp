#include "weapon/moving_tile/moving_tile_analysis.hpp"

#include "object/player.hpp"
#include "weapon/hurt.hpp"
#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"

MovingTileAnalysis::MovingTileAnalysis(const std::string& filename) :
	MovingTile(filename),
	m_wall_bounce_count()
{}

std::unique_ptr<MovingTile> MovingTileAnalysis::from_file(const ReaderData* data) {
	int bounce_wall_count = 0;
	data->get("bounce-wall-count", bounce_wall_count);
	bounce_wall_count = std::max(bounce_wall_count, 0);
	
	int damage = 1;
	data->get("damage", damage);
	damage = std::max(damage, 1);
	
	float speed = 100.0f;
	data->get("speed", speed);
	speed = std::max(speed, 100.0f);
	
	std::string movingtile_filename;
	if (!data->get("filename", movingtile_filename)) {
		throw std::runtime_error("MovingTile doesn't exist !!");
	}

	auto movingtile = std::make_unique<MovingTileAnalysis>(data->m_parent_path + movingtile_filename);
	movingtile->m_damage = damage;
	movingtile->m_wall_bounce_count = bounce_wall_count;
	movingtile->m_physic.set_velocity(Vector(speed, 0.0f));
	return movingtile;
}

void MovingTileAnalysis::collision_solid(const CollisionHit& hit) {
	if (m_wall_bounce_count > 0) {
		-- m_wall_bounce_count;
		if (hit.left || hit.right) {
			m_physic.set_inverse_velocity_x();
		}
		if (hit.top || hit.bottom) {
			m_physic.set_inverse_velocity_y();
		}
	}
	else {
		remove_me();
	}
}

HitResponse MovingTileAnalysis::collision(CollisionObject& other, const CollisionHit& /* hit */) {
	if (dynamic_cast<Player*>(&other)) {
		if (!(m_hurt_attributes & HURT_PLAYER)) {
			return ABORT_MOVE;
		}
		remove_me();
		return FORCE_MOVE;
	}

	if (dynamic_cast<MovingTile*>(&other)) {
		return ABORT_MOVE;
	}

	return CONTINUE;
}

void MovingTileAnalysis::collision_tile(uint32_t /* tile_attributes */) {}

std::string MovingTileAnalysis::class_name() { return "moving_tile_analysis"; }
std::string MovingTileAnalysis::get_class_name() const { return class_name(); }

#include "util/log.hpp"
#include "math/util.hpp"

std::unique_ptr<MovingTile> MovingTileAnalysis::clone(const Vector& pos, uint32_t hurt_attributes, float angle) const {
	auto movingtile = std::make_unique<MovingTileAnalysis>(m_sprite_name);
	movingtile->set_pos(pos);
	movingtile->set_hurt_attributes(hurt_attributes);
	movingtile->set_angle(angle);
	movingtile->m_wall_bounce_count = m_wall_bounce_count;

	Vector velocity = Vector(math::length(m_physic.get_velocity()), 0.0f);;
	// log_info << angle << ' ' << m_physic.get_velocity() << ' ' << math::rotate(velocity, angle) << '\n';
	// log_info << math::sin_degree(135.0f) << ' ' << math::sin_degree(-135.0f) << '\n';
	movingtile->m_physic.set_velocity(math::rotate(velocity, angle));
	movingtile->m_damage = m_damage;
	movingtile->m_ratio_crit = m_ratio_crit;

	return movingtile;
}