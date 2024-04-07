#include "badguy/ogre.hpp"

#include "asaed/room.hpp"
#include "math/random.hpp"
#include "math/util.hpp"
#include "sprite/sprite.hpp"
#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "weapon/weapon.hpp"
#include "weapon/weapon_set.hpp"

namespace {
	const float WALK_SPEED = 40.0f; // option
} // namespace

Ogre::Ogre(const std::string& filename) :
	BadGuy(filename)
{
	m_weapon = WeaponSet::current()->get(1).clone(this);
	m_weapon->set_offset(m_weapon->get_bounding_box().get_middle());
	set_action("idle-right");
}

#include "util/log.hpp"

void Ogre::update(float dt_sec) {
	if (m_health <= 0) {
		set_state(STATE_INACTIVE);
		deactivated();
		return;
	}

	switch (m_state) {
		case STATE_INACTIVE:
			inactive_update(dt_sec);
			try_active();
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

void Ogre::draw(DrawingContext& drawing_context) {
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
	}
	
	MovingSprite::draw(drawing_context);
	if (m_health > 0 && m_weapon) {
		m_weapon->draw(drawing_context);
	}
}

void Ogre::wandering() {
	if (math::distance(get_bounding_box().get_middle(), m_start_position) > m_radius_wander) {
		Vector to_rotate = m_start_position - get_bounding_box().get_middle();
		float angle = math::angle(to_rotate);
		m_physic.set_velocity(math::rotate(Vector(1.0f, 0.0f), angle) * WALK_SPEED);
	} 
	else if (m_timer_wander.check()) {
		m_physic.set_velocity(Vector(g_game_random.randf(-1.0f, 1.0f), g_game_random.randf(-1.0f, 1.0f)) * WALK_SPEED);
	}
}

void Ogre::activated() {

}

void Ogre::deactivated() {
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

void Ogre::active_update(float dt_sec) {
	const auto* player = Room::get().get_nearest_player(get_bounding_box().get_middle());
	if (!player) return;
	
	
}

void Ogre::inactive_update(float dt_sec) {
	if (m_health <= 0) return;
	wandering();
}

std::unique_ptr<BadGuy> Ogre::from_file(const ReaderData* data) {
	std::string filename;
	if (!data->get("filename", filename)) {
		throw std::runtime_error("Missing badguy data");
	}
	auto badguy = std::make_unique<Ogre>(data->m_parent_path + filename);
	
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

	data->get("radius-detect", badguy->m_radius_detect);

	float sec_per_shoot = 2.0f;
	data->get("sec-per-shoot", sec_per_shoot);
	badguy->m_timer_shoot.start(sec_per_shoot, true);

	return badguy;
}

std::unique_ptr<BadGuy> Ogre::clone(const Vector& pos) const {
	auto badguy = std::make_unique<Ogre>(m_sprite_name);
	badguy->m_physic.set_velocity(Vector(g_game_random.randf(-1.0f, 1.0f), g_game_random.randf(-1.0f, 1.0f)) * WALK_SPEED);

	badguy->set_pos(pos);
	badguy->m_start_position = badguy->get_bounding_box().get_middle();

	badguy->m_health = m_health;
	badguy->m_timer_dead.start(m_timer_dead.get_period(), true);
	badguy->m_timer_dead.pause();

	badguy->m_radius_wander = m_radius_wander;
	badguy->m_timer_wander.start(m_timer_wander.get_period(), true);

	badguy->m_radius_detect = m_radius_detect;

	badguy->m_timer_shoot.start(m_timer_shoot.get_period(), true);
	return badguy;
}