#include "weapon/projectile/projectile_big.hpp"

#include "asaed/room.hpp"
#include "object/player.hpp"
#include "badguy/badguy.hpp"
#include "math/random.hpp"
#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"
#include "weapon/hurt.hpp"
#include "weapon/projectile/projectile_line.hpp"
#include "weapon/projectile/projectile_set.hpp"

namespace {
	const float TIME_SPAWN = 0.3f; // options

	const int NUMBER_MINI_SPAWN = 8; // options
	const float CIRCLE = 360.0f; // options
} // namespace

ProjectileBig::ProjectileBig(const std::string& filename) :
	Projectile(filename),
	m_timer_spawn()
{}

std::unique_ptr<Projectile> ProjectileBig::from_file(const ReaderData* data) {
	int damage = 1;
	data->get("damage", damage);
	damage = std::max(damage, 1);
	
	float speed = 100.0f;
	data->get("speed", speed);
	speed = std::max(speed, 100.0f);

	int color = 1;
	data->get("color", color);
	color = std::max(color, 1);
	
	std::string projectile_filename;
	if (!data->get("filename", projectile_filename)) {
		throw std::runtime_error("Projectile doesn't exist !!");
	}

	auto projectile = std::make_unique<ProjectileBig>(data->m_parent_path + projectile_filename);
	projectile->m_damage = damage;
	projectile->m_physic.set_velocity(Vector(speed, 0.0f));
	return projectile;
}

void ProjectileBig::collision_tile(uint32_t /* tile_attributes */) {}

void ProjectileBig::update(float dt_sec) {
	if (!Room::get().inside(get_bounding_box())) {
		remove_me();
	}

	if (m_timer_spawn.check()) {
		const auto& projectile_mini = ProjectileSet::current()->get(MINI_PROJECTILE_BADGUY);
		float rotate_angle = CIRCLE / static_cast<float>(NUMBER_MINI_SPAWN);
		for (int i = 0; i < NUMBER_MINI_SPAWN; ++ i) {
			Room::get().add_object(std::move(projectile_mini.clone(get_bounding_box().get_middle(), m_hurt_attributes, get_angle() + static_cast<float>(i) * rotate_angle)));
		}
	}
	set_movement(m_physic.get_movement(dt_sec));
}

std::string ProjectileBig::class_name() { return "projectile_big"; }
std::string ProjectileBig::get_class_name() const { return class_name(); }

std::unique_ptr<Projectile> ProjectileBig::clone(const Vector& pos, uint32_t hurt_attributes, float angle, float /* angle_shift */) const {
	auto projectile = std::make_unique<ProjectileBig>(m_sprite_name);
	projectile->set_pos(pos);
	projectile->set_hurt_attributes(hurt_attributes);
	projectile->set_angle(angle);

	projectile->m_timer_spawn.start(TIME_SPAWN, true);

	Vector velocity = Vector(math::length(m_physic.get_velocity()), 0.0f);;
	projectile->m_physic.set_velocity(math::rotate(velocity, angle));
	projectile->m_damage = m_damage;
	projectile->m_ratio_crit = m_ratio_crit;

	return projectile;
}