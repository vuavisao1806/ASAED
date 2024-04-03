#include "weapon/shooting/shooting_analysis.hpp"

#include "util/reader_data.hpp"
#include "util/reader_machine.hpp"

ShootingAnalysis::ShootingAnalysis(const std::string& filename) :
	Shooting(filename),
	m_moving_tile_id(0),
	m_spawn_position(Vector(0.0f, 0.0f))
{}


std::unique_ptr<Weapon> ShootingAnalysis::from_file(const ReaderData* data) {
	int attack_per_turn = 1;
	data->get("attack-per-turn", attack_per_turn);
	attack_per_turn = std::max(attack_per_turn, 1);

	int id = 1;
	data->get("moving-tile-id", id);
	id = std::max(id, 1);

	float attack_per_sec = 1.0f;
	data->get("attack-per-sec", attack_per_sec);

	std::string weapon_filename;
	if (!data->get("filename", weapon_filename)) {
		throw std::runtime_error("Weapon doesn't exist !!");
	}
	
	auto weapon = std::make_unique<ShootingAnalysis>(data->m_parent_path + weapon_filename);
	weapon->m_moving_tile_id = id;
	weapon->m_attack_per_turn = attack_per_turn;
	weapon->m_timer.start(1.0f / attack_per_sec, true);
	return weapon;
}

uint32_t ShootingAnalysis::get_moving_tile_id() const { return m_moving_tile_id; }
Vector ShootingAnalysis::get_spawn_position() const { return get_bounding_box().get_middle(); }
float ShootingAnalysis::get_shoot_angle() const { return get_angle(); }; // temporary, will update

std::string ShootingAnalysis::class_name() { return "shooting-analysis"; }
std::string ShootingAnalysis::get_class_name() const { return class_name(); };

std::unique_ptr<Weapon> ShootingAnalysis::clone(MovingObject* parent, const Vector& pos) const {
	auto weapon = std::make_unique<ShootingAnalysis>(m_sprite_name);
	
	weapon->set_parent(parent);
	weapon->set_pos(pos);
	weapon->m_offset = m_offset;
	weapon->m_hurt_attributes = m_hurt_attributes;

	weapon->m_attack_per_turn = m_attack_per_turn;
	weapon->m_timer.start(m_timer.get_period(), true);

	weapon->m_spawn_position = m_spawn_position;
	weapon->m_moving_tile_id = m_moving_tile_id;
	return weapon;
}