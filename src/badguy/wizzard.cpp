#include "badguy/wizzard.hpp"

#include "asaed/room.hpp"
#include "object/player.hpp"
#include "math/random.hpp"
#include "math/util.hpp"
#include "sprite/sprite.hpp"
#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "weapon/weapon.hpp"
#include "weapon/weapon_set.hpp"

namespace {
	const float WALK_SPEED = 35.0f; // option
} // namespace

Wizzard::Wizzard(const std::string& filename) :
	BadGuy(filename)
{
	m_weapon = WeaponSet::current()->get(2).clone(this);
	m_weapon->set_offset(m_weapon->get_bounding_box().get_middle());
	set_action("idle-right");
}

void Wizzard::update(float dt_sec) {
	if (m_health <= 0) {
		set_state(STATE_INACTIVE);
		deactivated();
		return;
	}

	try_active();

	switch (m_state) {
		case STATE_INACTIVE:
			inactive_update(dt_sec);
			break;
		case STATE_ACTIVE:
			active_update(dt_sec);
			break;
		
		default:
			throw std::runtime_error("Unknown state bad guy");
			break;
	}

	if (m_physic.get_velocity_x() < 0.0f) {
		m_direction = Direction::LEFT;
	}
	else {
		m_direction = Direction::RIGHT;
	}
	set_movement(m_physic.get_movement(dt_sec));
}

void Wizzard::draw(DrawingContext& drawing_context) {
	if (m_health <= 0) {
		float angle = (m_direction == Direction::RIGHT ? 90.0f : -90.0f);
		m_sprite->set_angle(angle);
		m_sprite->set_color(Color(0.5f, 0.5f, 0.5f));
	}
	else {
		std::string suffix_action = (m_direction == Direction::RIGHT ? "-right" : "-left");
		if (math::length(m_physic.get_velocity()) < 1.0f) {
			m_sprite->set_action("idle" + suffix_action);
		}
		else {
			m_sprite->set_action("walk" + suffix_action);
		}

		if (m_weapon) {
			m_weapon->draw(drawing_context);
		}
	}
	
	MovingSprite::draw(drawing_context);
}

void Wizzard::wandering() {
	if (math::distance(get_bounding_box().get_middle(), m_start_position) > m_radius_wander) {
		Vector to_rotate = m_start_position - get_bounding_box().get_middle();
		float angle = math::angle(to_rotate);
		m_physic.set_velocity(math::rotate(Vector(1.0f, 1.0f), angle) * WALK_SPEED);
	} 
	else if (m_timer_wander.check()) {
		m_physic.set_velocity(math::rotate(Vector(1.0f, 1.0f), g_game_random.randf(0.0f, 360.f)) * WALK_SPEED);
	}
}

void Wizzard::activated() {}

void Wizzard::deactivated() {
	set_group(COLLISION_DISABLED);
	m_physic.set_velocity(Vector(0.0f, 0.0f));

	if (m_timer_dead.paused()) {
		m_timer_dead.resume();
		std::string suffix_action = (m_direction == Direction::RIGHT ? "-right" : "-left");
		set_action("dead" + suffix_action);
	}

	if (m_timer_dead.check()) {
		remove_me();
	}
}

void Wizzard::active_update(float /* dt_sec */) {
	if (m_timer_shoot.paused()) {
		m_timer_shoot.start_with_previous();
	}

	auto player_ptr = Room::get().get_nearest_player(m_bounding_box.get_middle());
	Vector to_rotate = player_ptr->get_bounding_box().get_middle() - m_bounding_box.get_middle();
	float dist = math::length(to_rotate);

	if (dist <= m_radius_wander && g_game_random.test_lucky()) {
		int try_new_start_position = g_game_random.rand(1, 4);
		const Rectf rect = Room::get().get_bounding_box();
		for (int i = 1; i < try_new_start_position; ++ i) {
			Vector new_start_position = Vector(g_game_random.randf(rect.get_left(), rect.get_right()),
			                                   g_game_random.randf(rect.get_top(), rect.get_bottom()));
			Rectf rect = Rectf(new_start_position, get_bounding_box().get_size());
			if (!Room::get().is_free_of_tiles(rect)) {
				continue;
			}
			float new_dist = math::distance(rect.get_middle(), player_ptr->get_bounding_box().get_middle());
			if (new_dist > dist) {
				dist = new_dist;
				set_start_position(rect.get_middle());
			}
		}
	}

	wandering();

	float angle = math::angle(to_rotate);
	if (m_weapon) {
		m_weapon->set_angle(angle);
	}

	if (std::fabs(angle) >= 90.0f) {
		m_weapon->set_flip(VERTICAL_FLIP);
	}
	else {
		m_weapon->set_flip(NO_FLIP);
	}

	if (m_timer_shoot.check()) {
		m_weapon->attack();
	}
}

void Wizzard::inactive_update(float /* dt_sec */) {
	if (m_health <= 0) return;

	if (m_timer_shoot.get_period() != 0.0f) {
		m_timer_shoot.pause_with_previous();
	}

	wandering();

	if (m_physic.get_velocity_x() < 0.0f) {
		m_weapon->set_flip(HORIZONTAL_FLIP);
	}
	else{
		m_weapon->set_flip(NO_FLIP);
	}
}

std::unique_ptr<BadGuy> Wizzard::from_file(const ReaderData* data) {
	std::string filename;
	if (!data->get("filename", filename)) {
		throw std::runtime_error("Missing badguy data");
	}
	auto badguy = std::make_unique<Wizzard>(data->m_parent_path + filename);
	
	if (!data->get("health", badguy->m_health)) {
		throw std::runtime_error("Missing badguy health");
	}

	float sec_per_dead = 2.0f;
	data->get("sec-per-dead", sec_per_dead);
	badguy->m_timer_dead.start(sec_per_dead, true);

	data->get("radius-wander", badguy->m_radius_wander);

	float sec_per_wander = 2.0f;
	data->get("sec-per-wander", sec_per_wander);
	badguy->m_timer_wander.start(sec_per_wander, true);

	float sec_per_shoot = 2.0f;
	data->get("sec-per-shoot", sec_per_shoot);
	badguy->m_timer_shoot.start(sec_per_shoot, true);

	return badguy;
}

bool Wizzard::is_boss() const { return false; }

std::unique_ptr<BadGuy> Wizzard::clone(const Vector& pos) const {
	auto badguy = std::make_unique<Wizzard>(m_sprite_name);
	badguy->m_physic.set_velocity(Vector(g_game_random.randf(-1.0f, 1.0f), g_game_random.randf(-1.0f, 1.0f)) * WALK_SPEED);

	badguy->set_pos(pos);
	badguy->m_start_position = badguy->get_bounding_box().get_middle();

	badguy->m_health = m_health;
	badguy->m_timer_dead.start(m_timer_dead.get_period(), true);
	badguy->m_timer_dead.pause_with_previous();

	badguy->m_radius_wander = m_radius_wander;
	badguy->m_timer_wander.start(m_timer_wander.get_period(), true);

	badguy->m_timer_shoot.start(m_timer_shoot.get_period(), true);
	badguy->m_timer_shoot.pause_with_previous();
	return badguy;
}