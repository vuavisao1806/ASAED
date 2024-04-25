#include "badguy/zulan.hpp"

#include "asaed/room.hpp"
#include "object/camera.hpp"
#include "object/player.hpp"
#include "math/random.hpp"
#include "math/util.hpp"
#include "gui/colorscheme.hpp"
#include "sprite/sprite.hpp"
#include "util/a_start.hpp"
#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"
#include "weapon/hurt.hpp"
#include "weapon/projectile/projectile_set.hpp"
#include "weapon/projectile/projectile_line.hpp"

namespace {
	const float SKILL_RECOVERY_1 = 7.5f; // option
	const float SKILL_RECOVERY_2 = 15.0f; // option
	const float SKILL_RECOVERY_3 = 7.5f; // option
	const float HEALTH_BAR_WIDTH = 200.0f; // option
	const float HEALTH_BAR_HEIGHT = 12.5f; // option

	const int NUMBER_SHOOTING_SKILL_1 = 30; // option

	const float SKILL_RECOVERY_SHOOTING_SKILL_2 = 0.15f; // option
	const float DELTA_SHOOTING_SKILL_2 = 10.0f; // option
	const int NUMBER_SHOOTING_SKILL_2 = 36; // option
	
	const float CIRCLE = 360.0f;
} // namespace

Zulan::Zulan(const std::string& filename) :
	BadGuy(filename),
	m_timer_skill_1(),
	m_timer_skill_2(),
	m_cnt_shooting_skill_2(0),
	m_angle_shooting_skill_2(0.0f),
	m_timer_skill_3()
{}

void Zulan::update(float dt_sec) {
	if (m_health <= 0) {
		set_state(STATE_INACTIVE);
		deactivated();
		return;
	}
	// boss is auto active
	active_update(dt_sec);
}

void Zulan::draw(DrawingContext& drawing_context) {
	if (m_health <= 0) {
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

	// draw HUD
	{
		m_health = std::max(m_health, 0);

		Vector position = Room::get().get_camera().get_translation() + 
		                  Vector(Room::get().get_camera().get_screen_size().width / 2.0f, 3.0f) -
		                  Vector(HEALTH_BAR_WIDTH / 2.0f, 0.0f);

		Sizef size = Sizef(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);
		
		drawing_context.get_canvas().draw_filled_rect(Rectf(position, size), ColorScheme::HUD::frame_back, LAYER_HUD);
		position += Vector(1.0f, 1.0f);

		size -= Sizef(2.0f, 2.0f);
		drawing_context.get_canvas().draw_filled_rect(Rectf(position, size), ColorScheme::HUD::frame_front, LAYER_HUD);

		position += Vector(1.0f, 1.0f);
		size -= Sizef(2.0f, 2.0f);
		
		drawing_context.get_canvas().draw_filled_rect(Rectf(position, size), ColorScheme::HUD::space_back, LAYER_HUD);
		
		position += Vector(1.0f, 1.0f);
		size -= Sizef(2.0f, 2.0f);

		float rem = size.width / HEALTH;
		size.height /= 2.0f;
		drawing_context.get_canvas().draw_filled_rect(Rectf(position, size - Sizef(rem, 0.0f) * (HEALTH - m_health)), ColorScheme::HUD::heart_front, LAYER_HUD);

		position.y += size.height;
		drawing_context.get_canvas().draw_filled_rect(Rectf(position, size - Sizef(rem, 0.0f) * (HEALTH - m_health)), ColorScheme::HUD::heart_back, LAYER_HUD);
	}
	
	MovingSprite::draw(drawing_context);
}

void Zulan::wandering() {}

void Zulan::activated() {}

void Zulan::deactivated() {
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

void Zulan::active_update(float /* dt_sec */) {
	if (m_timer_skill_1.paused()) {
		m_timer_skill_1.start_with_previous();
	}
	if (m_timer_skill_2.paused()) {
		m_timer_skill_2.start_with_previous();
	}
	if (m_timer_skill_3.paused()) {
		m_timer_skill_3.start_with_previous();
	}

	wandering();

	if (m_timer_skill_1.check()) {
		skill_1();
	}
	
	if (m_timer_skill_2.check() && m_cnt_shooting_skill_2 == 0) {
		m_angle_shooting_skill_2 = g_game_random.randf(0.0f, 360.0f);
		m_cnt_shooting_skill_2 = NUMBER_SHOOTING_SKILL_2;
		m_timer_recovery_shooting_skill_2.start(SKILL_RECOVERY_SHOOTING_SKILL_2, true);
	}

	if (m_cnt_shooting_skill_2 != 0 && m_timer_recovery_shooting_skill_2.check()) {
		skill_2();
	}

	if (m_timer_skill_3.check()) {
		skill_3();
	}
}
void Zulan::inactive_update(float /* dt_sec */) {}

void Zulan::skill_1() {
	float delta = CIRCLE / NUMBER_SHOOTING_SKILL_1;

	float angle = g_game_random.randf(0.0f, 360.0f);
	const auto& projectile = ProjectileSet::current()->get(SPECIAL_PROJECTILE_BADGUY);
	
	for (int i = 0; i < NUMBER_SHOOTING_SKILL_1; ++ i) {
		Room::get().add_object(std::move(projectile.clone(get_bounding_box().get_middle(), HURT_PLAYER, angle)));
		angle += delta;
	}
}

void Zulan::skill_2() {
	const auto& projectile = ProjectileSet::current()->get(SPECIAL_PROJECTILE_BADGUY);
	for (int i = 0; i < 4; ++ i) {
		float angle = m_angle_shooting_skill_2 + static_cast<float>(i) * 90.0f;
		Room::get().add_object(std::move(projectile.clone(get_bounding_box().get_middle(), HURT_PLAYER, angle)));
	}

	m_angle_shooting_skill_2 += DELTA_SHOOTING_SKILL_2;
	-- m_cnt_shooting_skill_2;
}

void Zulan::skill_3() {

}

std::unique_ptr<BadGuy> Zulan::from_file(const ReaderData* data) {
	std::string filename;
	if (!data->get("filename", filename)) {
		throw std::runtime_error("Missing badguy data");
	}
	auto badguy = std::make_unique<Zulan>(data->m_parent_path + filename);
	
	if (!data->get("health", badguy->m_health)) {
		throw std::runtime_error("Missing badguy health");
	}

	float sec_per_wander = 2.0f;
	data->get("sec-per-wander", sec_per_wander);
	badguy->m_timer_wander.start(sec_per_wander, true);

	float sec_per_dead = 2.0f;
	data->get("sec-per-dead", sec_per_dead);
	badguy->m_timer_dead.start(sec_per_dead, true);

	return badguy;
}

bool Zulan::is_boss() const { return true; }

std::unique_ptr<BadGuy> Zulan::clone(const Vector& pos) const {
	auto badguy = std::make_unique<Zulan>(m_sprite_name);

	badguy->set_pos(pos);
	badguy->m_start_position = badguy->get_bounding_box().get_middle();

	badguy->m_health = m_health;
	badguy->HEALTH = m_health;
	badguy->m_timer_dead.start(m_timer_dead.get_period(), true);
	badguy->m_timer_dead.pause_with_previous();

	badguy->m_timer_skill_1.start(SKILL_RECOVERY_1, true);
	badguy->m_timer_skill_1.pause_with_previous();

	badguy->m_timer_skill_2.start(SKILL_RECOVERY_2, true);
	badguy->m_timer_skill_2.pause_with_previous();
	
	badguy->m_timer_skill_3.start(SKILL_RECOVERY_3, true);
	badguy->m_timer_skill_3.pause_with_previous();
	return badguy;
}