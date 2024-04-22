#include "weapon/projectile/projectile_cycle.hpp"

#include "asaed/room.hpp"
#include "object/player.hpp"
#include "badguy/badguy.hpp"
#include "weapon/hurt.hpp"
#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"

namespace {
	const float SHIFT = 1.5f; // options
}

ProjectileCycle::ProjectileCycle(const std::string& filename) :
	Projectile(filename),
	m_shift(0.0f, 0.0f)
{}

std::unique_ptr<Projectile> ProjectileCycle::from_file(const ReaderData* data) {
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

	auto projectile = std::make_unique<ProjectileCycle>(data->m_parent_path + projectile_filename);
	projectile->m_damage = damage;
	projectile->m_physic.set_velocity(Vector(speed, 0.0f));
	return projectile;
}


void ProjectileCycle::collision_solid(const CollisionHit& /* hit */) {
	remove_me();
}

void ProjectileCycle::collision_tile(uint32_t /* tile_attributes */) {}

void ProjectileCycle::update(float dt_sec) {
	if (!Room::get().inside(get_bounding_box())) {
		remove_me();
	}
	m_physic.set_velocity(math::rotate(m_physic.get_velocity(), dt_sec * 360.0f));
	set_movement(m_physic.get_movement(dt_sec) + m_shift);
}

std::string ProjectileCycle::class_name() { return "projectile_cycle"; }
std::string ProjectileCycle::get_class_name() const { return class_name(); }

#include "util/log.hpp"
std::unique_ptr<Projectile> ProjectileCycle::clone(const Vector& pos, uint32_t hurt_attributes, float angle) const {
	auto projectile = std::make_unique<ProjectileCycle>(m_sprite_name);
	projectile->set_pos(pos);
	projectile->set_hurt_attributes(hurt_attributes);
	projectile->set_angle(angle);

	Vector velocity = Vector(math::length(m_physic.get_velocity()), 0.0f);;
	projectile->m_physic.set_velocity(math::rotate(velocity, angle));
	projectile->m_shift = Vector(math::rotate(Vector(SHIFT, 0.0f), 0.0f));
	// log_warning << angle << '\n';
	// log_warning << projectile->m_shift << '\n';

	projectile->m_damage = m_damage;
	projectile->m_ratio_crit = m_ratio_crit;

	return projectile;
}