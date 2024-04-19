#include "weapon/projectile/projectile_line.hpp"

#include "object/player.hpp"
#include "badguy/badguy.hpp"
#include "weapon/hurt.hpp"
#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"

ProjectileLine::ProjectileLine(const std::string& filename) :
	Projectile(filename),
	m_wall_bounce_count()
{}

std::unique_ptr<Projectile> ProjectileLine::from_file(const ReaderData* data) {
	int bounce_wall_count = 0;
	data->get("bounce-wall-count", bounce_wall_count);
	bounce_wall_count = std::max(bounce_wall_count, 0);
	
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

	auto projectile = std::make_unique<ProjectileLine>(data->m_parent_path + projectile_filename);
	projectile->m_damage = damage;
	projectile->m_wall_bounce_count = bounce_wall_count;
	projectile->m_physic.set_velocity(Vector(speed, 0.0f));
	return projectile;
}


void ProjectileLine::collision_solid(const CollisionHit& hit) {
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

void ProjectileLine::collision_tile(uint32_t /* tile_attributes */) {}

std::string ProjectileLine::class_name() { return "projectile_line"; }
std::string ProjectileLine::get_class_name() const { return class_name(); }

std::unique_ptr<Projectile> ProjectileLine::clone(const Vector& pos, uint32_t hurt_attributes, float angle) const {
	auto projectile = std::make_unique<ProjectileLine>(m_sprite_name);
	projectile->set_pos(pos);
	projectile->set_hurt_attributes(hurt_attributes);
	projectile->set_angle(angle);
	projectile->m_wall_bounce_count = m_wall_bounce_count;

	Vector velocity = Vector(math::length(m_physic.get_velocity()), 0.0f);;
	projectile->m_physic.set_velocity(math::rotate(velocity, angle));
	projectile->m_damage = m_damage;
	projectile->m_ratio_crit = m_ratio_crit;

	return projectile;
}