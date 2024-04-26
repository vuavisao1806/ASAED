#include "weapon/projectile/projectile_spawn.hpp"

#include "asaed/room.hpp"
#include "object/player.hpp"
#include "badguy/badguy.hpp"
#include "badguy/badguy_manager.hpp"
#include "math/random.hpp"
#include "weapon/hurt.hpp"
#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"

namespace {
	const float TIME_STOP = 1.5f;
	const float TIME_VALID = 3.5f;
	const float TIME_SPAWN = 0.5f;

	const int MIN_SPAWN = 1;
	const int MAX_SPAWN = 4;
	
	const int NUMBER_BADGUY = 3;
	const std::string listBadguy[] = {"ogre", "wizzard", "orc"};
}

ProjectileSpawn::ProjectileSpawn(const std::string& filename) :
	Projectile(filename),
	m_timer_stop(),
	m_timer_valid(),
	m_cnt_spawn(0),
	m_timer_spawn()
{}

std::unique_ptr<Projectile> ProjectileSpawn::from_file(const ReaderData* data) {
	int damage = 1;
	data->get("damage", damage);
	
	float speed = 100.0f;
	data->get("speed", speed);
	speed = std::max(speed, 100.0f);
	
	std::string projectile_filename;
	if (!data->get("filename", projectile_filename)) {
		throw std::runtime_error("Projectile doesn't exist !!");
	}

	auto projectile = std::make_unique<ProjectileSpawn>(data->m_parent_path + projectile_filename);
	projectile->m_damage = damage;
	projectile->m_physic.set_velocity(Vector(speed, 0.0f));
	return projectile;
}


void ProjectileSpawn::collision_solid(const CollisionHit& /* hit */) {
	m_physic.set_velocity(Vector(0.0f, 0.0f));
	if (m_timer_stop.check()) {
		m_physic.set_velocity(Vector(0.0f, 0.0f));
		m_cnt_spawn = g_game_random.rand(MIN_SPAWN, MAX_SPAWN);
		m_timer_valid.start(TIME_VALID, false);
		m_timer_spawn.start(TIME_SPAWN, true);
	}
}


HitResponse ProjectileSpawn::collision(CollisionObject& /* other */, const CollisionHit& /* hit */) { return ABORT_MOVE; }

void ProjectileSpawn::collision_tile(uint32_t /* tile_attributes */) {}

void ProjectileSpawn::update(float dt_sec) {
	if (!Room::get().inside(get_bounding_box())) {
		remove_me();
	}
	if (m_timer_stop.check()) {
		m_physic.set_velocity(Vector(0.0f, 0.0f));
		m_cnt_spawn = g_game_random.rand(MIN_SPAWN, MAX_SPAWN);
		m_timer_valid.start(TIME_VALID, false);
		m_timer_spawn.start(TIME_SPAWN, true);
	}

	if (m_timer_valid.check()) {
		remove_me();
	}

	if (m_cnt_spawn > 0 && m_timer_spawn.check()) {
		-- m_cnt_spawn;
		int id = g_game_random.rand(0, NUMBER_BADGUY - 1);
		
		const auto& badguy = BadGuyManager::current()->get(listBadguy[id]);
		Rectf rect = Rectf(get_bounding_box().p1(), badguy.get_bounding_box().get_size());
		if (Room::get().inside(rect) && Room::get().is_free_of_tiles(rect)) {
			Room::get().add_object(badguy.clone(get_bounding_box().p1()));
		}
	}
	
	set_movement(m_physic.get_movement(dt_sec));
}

std::string ProjectileSpawn::class_name() { return "projectile_spawn"; }
std::string ProjectileSpawn::get_class_name() const { return class_name(); }

std::unique_ptr<Projectile> ProjectileSpawn::clone(const Vector& pos, uint32_t hurt_attributes, float angle, float /* angle_shift */) const {
	auto projectile = std::make_unique<ProjectileSpawn>(m_sprite_name);
	projectile->set_pos(pos);
	projectile->set_hurt_attributes(hurt_attributes);
	projectile->set_angle(angle);

	Vector velocity = Vector(math::length(m_physic.get_velocity()), 0.0f);;
	projectile->m_physic.set_velocity(math::rotate(velocity, angle));

	projectile->m_timer_stop.start(TIME_STOP, false);

	projectile->m_damage = m_damage;
	projectile->m_ratio_crit = m_ratio_crit;

	return projectile;
}