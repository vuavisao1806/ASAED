#include "weapon/projectile/projectile_cycloid.hpp"

#include "asaed/room.hpp"
#include "badguy/badguy.hpp"
#include "math/util.hpp"
#include "object/player.hpp"
#include "weapon/hurt.hpp"
#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"

namespace {
	const float SPEED = 100.0f;
	const float RADIUS = 15.0f;
} // namespace

ProjectileCycloid::ProjectileCycloid(const std::string& filename) :
	Projectile(filename),
	m_first_position(),
	m_timer(0.0f)
{}

std::unique_ptr<Projectile> ProjectileCycloid::from_file(const ReaderData* data) {
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

	auto projectile = std::make_unique<ProjectileCycloid>(data->m_parent_path + projectile_filename);
	projectile->m_damage = damage;
	projectile->m_physic.set_velocity(Vector(speed, 0.0f));
	return projectile;
}

void ProjectileCycloid::collision_solid(const CollisionHit& /* hit */) {
	remove_me();
}

void ProjectileCycloid::collision_tile(uint32_t /* tile_attributes */) {}

#include "util/log.hpp"
void ProjectileCycloid::update(float dt_sec) {
	if (!Room::get().inside(get_bounding_box())) {
		remove_me();
	}
	m_timer += 5.0f * dt_sec;
	// Vector new_position = m_first_position + math::rotate(Vector(m_timer - math::sin_degree(m_timer), 1.0f - math::cos_degree(m_timer)) * RADIUS, get_angle());
	Vector new_position = m_first_position + Vector(m_timer - math::sin_degree(math::randians_to_degrees(m_timer)), 1.0f - math::cos_degree(math::randians_to_degrees(m_timer))) * RADIUS;
	// log_warning << "first: " << m_first_position << " to new: " << ' ' << Vector(m_timer - math::sin_degree(math::randians_to_degrees(m_timer)), 1.0f - math::cos_degree(math::randians_to_degrees(m_timer))) * RADIUS << ' ' << new_position << '\n';
	Vector cur_position = get_pos();
	// log_warning << "cur: " << cur_position << '\n';
	m_physic.set_velocity(math::rotate(Vector(SPEED, 0.0f), math::angle(new_position - cur_position)));
	set_movement(m_physic.get_movement(dt_sec));
}

std::string ProjectileCycloid::class_name() { return "projectile_cycloid"; }
std::string ProjectileCycloid::get_class_name() const { return class_name(); }

std::unique_ptr<Projectile> ProjectileCycloid::clone(const Vector& pos, uint32_t hurt_attributes, float angle, float /* angle_shift */) const {
	auto projectile = std::make_unique<ProjectileCycloid>(m_sprite_name);
	projectile->set_pos(pos);
	projectile->set_hurt_attributes(hurt_attributes);
	projectile->set_angle(angle);

	projectile->m_first_position = projectile->get_pos();
	log_warning << projectile->get_pos() << '\n';
	projectile->m_timer = 0.0f;

	Vector velocity = Vector(math::length(m_physic.get_velocity()), 0.0f);;
	// projectile->m_physic.set_velocity(math::rotate(velocity, angle));
	projectile->m_physic.set_velocity(math::rotate(velocity, 0.0f)); // tested
	projectile->m_damage = m_damage;
	projectile->m_ratio_crit = m_ratio_crit;

	return projectile;
}