#include "weapon/projectile/projectile_boomerang.hpp"

#include "asaed/room.hpp"
#include "object/player.hpp"
#include "badguy/badguy.hpp"
#include "weapon/hurt.hpp"
#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"

namespace {
	const float TIME_GO_BACK = 1.0f;
} // namespace

ProjectileBoomerang::ProjectileBoomerang(const std::string& filename) :
	Projectile(filename),
	m_turn(0),
	m_turn_damage(2),
	m_timer_go_back()
{}

std::unique_ptr<Projectile> ProjectileBoomerang::from_file(const ReaderData* data) {
	int damage = 1;
	data->get("damage", damage);
	damage = std::max(damage, 1);
	
	float speed = 100.0f;
	data->get("speed", speed);
	speed = std::max(speed, 100.0f);
	
	std::string projectile_filename;
	if (!data->get("filename", projectile_filename)) {
		throw std::runtime_error("Projectile doesn't exist !!");
	}

	auto projectile = std::make_unique<ProjectileBoomerang>(data->m_parent_path + projectile_filename);
	projectile->m_damage = damage;
	projectile->m_physic.set_velocity(Vector(speed, 0.0f));
	return projectile;
}


void ProjectileBoomerang::collision_solid(const CollisionHit& /* hit */) {
	remove_me();
}

HitResponse ProjectileBoomerang::collision(CollisionObject& other, const CollisionHit& /* hit */) {
	if (dynamic_cast<Player*>(&other)) {
		-- m_turn_damage;
	}
	if (m_turn_damage <= 0) {
		m_damage = 0;
	}
}

void ProjectileBoomerang::collision_tile(uint32_t /* tile_attributes */) {}

void ProjectileBoomerang::update(float dt_sec) {
	if (!Room::get().inside(get_bounding_box())) {
		remove_me();
	}
	if (m_timer_go_back.check()) {
		m_physic.set_inverse_velocity_x();
		m_physic.set_inverse_velocity_y();
		++ m_turn;
		if (m_turn == 2) {
			remove_me();
		}
	}

	set_movement(m_physic.get_movement(dt_sec));
}

std::string ProjectileBoomerang::class_name() { return "projectile_boomerang"; }
std::string ProjectileBoomerang::get_class_name() const { return class_name(); }

std::unique_ptr<Projectile> ProjectileBoomerang::clone(const Vector& pos, uint32_t hurt_attributes, float angle, float /* angle_shift */) const {
	auto projectile = std::make_unique<ProjectileBoomerang>(m_sprite_name);
	projectile->set_pos(pos);
	projectile->set_hurt_attributes(hurt_attributes);
	projectile->set_angle(angle);

	Vector velocity = Vector(math::length(m_physic.get_velocity()), 0.0f);;
	projectile->m_physic.set_velocity(math::rotate(velocity, angle));

	projectile->m_timer_go_back.start(TIME_GO_BACK, true);
	projectile->m_turn_damage = m_turn_damage;

	projectile->m_damage = m_damage;
	projectile->m_ratio_crit = m_ratio_crit;

	return projectile;
}