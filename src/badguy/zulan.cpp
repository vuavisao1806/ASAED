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
	const float WALK_SPEED = 30.0f; // options
	const float SKILL_RECOVERY_1 = 3.5f; // options
	const float SKILL_RECOVERY_2 = 9.0f; // options
	const float SKILL_RECOVERY_3 = 7.5f; // options
	const float SKILL_RECOVERY_3_ANGRY = 9.5f; // options
	const float SKILL_RECOVERY_4 = 17.5f; // options

	const float HEALTH_BAR_WIDTH = 200.0f; // options
	const float HEALTH_BAR_HEIGHT = 12.5f; // options

	const int NUMBER_SHOOTING_SKILL_1 = 30; // options
	
	const float CIRCLE = 360.0f;
} // namespace

Zulan::Zulan(const std::string& filename) :
	BadGuy(filename),
	m_timer_skill_1(),
	m_timer_skill_2(),
	m_timer_skill_3(),
	m_timer_skill_4()
{}

void Zulan::update(float dt_sec) {
	if (m_health <= 0) {
		set_state(STATE_INACTIVE);
		deactivated();
		return;
	}
	// boss is auto active
	active_update(dt_sec);

	if (m_physic.get_velocity_x() < 0.0f) {
		m_direction = Direction::LEFT;
	}
	else {
		m_direction = Direction::RIGHT;
	}
	set_movement(m_physic.get_movement(dt_sec));
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

void Zulan::wandering() {
	if (m_timer_wander.check()) {
		m_physic.set_velocity(math::rotate(Vector(1.0f, 1.0f), g_game_random.randf(0.0f, 360.f)) * WALK_SPEED);
	}
}

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

	if (m_health < HEALTH / 2) {
		m_timer_skill_3_angry.start_with_previous();
		m_timer_skill_4.start_with_previous();
	}
	wandering();

	if (m_timer_skill_1.check()) {
		skill_1();
	}

	if (m_timer_skill_2.check()) {
		skill_2();
	}
	
	if (m_health >= HEALTH / 2) {
		if (m_timer_skill_3.check()) {
			skill_3();
		}
	}
	else {
		if (m_timer_skill_3_angry.check()) {
			skill_3_angry();
		}

		if (m_timer_skill_4.check()) {
			skill_4();
		}
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
	if (!Room::get().get_nearest_player(get_bounding_box().get_middle())) {
		return;
	}
	float angle = math::angle(Room::get().get_nearest_player(get_bounding_box().get_middle())->get_pos() - get_bounding_box().get_middle());
	
	const Projectile& projectile = ProjectileSet::current()->get(BIG_PROJECTILE_BADGUY);
	const Rectf rect = Rectf(get_bounding_box().get_middle(), projectile.get_bounding_box().get_size());
	if (Room::get().inside(rect)) {
		Room::get().add_object(std::move(projectile.clone(get_bounding_box().get_middle(), HURT_PLAYER, angle)));
	}
}

void Zulan::skill_3() {
	if (!Room::get().get_nearest_player(get_bounding_box().get_middle())) {
		return;
	}
	float angle_spawn = math::angle(Room::get().get_nearest_player(get_bounding_box().get_middle())->get_pos() - get_bounding_box().get_middle());
	const auto& projectile = ProjectileSet::current()->get(CYCLE_PROJECTILE_BADGUY);

	for (float angle = 0; angle < 360; angle += 20) {
		const Rectf rect = Rectf(get_bounding_box().get_middle(), projectile.get_bounding_box().get_size());
		if (Room::get().inside(rect)) {
			Room::get().add_object(projectile.clone(get_bounding_box().get_middle(), HURT_PLAYER, angle, angle_spawn));
		}
	}
}

void Zulan::skill_3_angry() {
	float angle_spawn = g_game_random.randf(0.0f, 360.0f);
	const auto& projectile = ProjectileSet::current()->get(CYCLE_PROJECTILE_BADGUY);

	for (int i = 0; i < 3; ++ i) {
		for (float angle = 0; angle < 360; angle += 40) {
			const Rectf rect = Rectf(get_bounding_box().get_middle(), projectile.get_bounding_box().get_size());
			if (Room::get().inside(rect)) {
				Room::get().add_object(projectile.clone(get_bounding_box().get_middle(), HURT_PLAYER, angle, angle_spawn));
			}
		}
		angle_spawn += 120.0f;
		angle_spawn = std::fmod(angle_spawn, 360.0f);
	}
}

void Zulan::skill_4() {
	float angle_spawn = g_game_random.randf(0.0f, 360.0f);
	const auto& projectile = ProjectileSet::current()->get(SPAWN_PROJECTILE_BADGUY);
	for (int i = 0; i < 3; ++ i) {
		const Rectf rect = Rectf(get_bounding_box().get_middle(), projectile.get_bounding_box().get_size());
		if (Room::get().inside(rect)) {
			Room::get().add_object(projectile.clone(get_bounding_box().get_middle(), HURT_PLAYER, angle_spawn));
		}
		angle_spawn += 120.0f;
		angle_spawn = std::fmod(angle_spawn, 360.0f);
	}
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
	badguy->m_physic.set_velocity(math::rotate(Vector(1.0f, 1.0f), g_game_random.randf(0.0f, 360.f)) * WALK_SPEED);

	badguy->m_health = m_health;
	badguy->HEALTH = m_health;
	badguy->m_timer_dead.start(m_timer_dead.get_period(), true);
	badguy->m_timer_dead.pause_with_previous();

	badguy->m_timer_wander.start(m_timer_wander.get_period(), true);

	badguy->m_timer_skill_1.start(SKILL_RECOVERY_1, true);
	badguy->m_timer_skill_1.pause_with_previous();

	badguy->m_timer_skill_2.start(SKILL_RECOVERY_2, true);
	badguy->m_timer_skill_2.pause_with_previous();
	
	badguy->m_timer_skill_3.start(SKILL_RECOVERY_3, true);
	badguy->m_timer_skill_3.pause_with_previous();

	badguy->m_timer_skill_3_angry.start(SKILL_RECOVERY_3_ANGRY, true);
	badguy->m_timer_skill_3_angry.pause_with_previous();

	badguy->m_timer_skill_4.start(SKILL_RECOVERY_4, true);
	badguy->m_timer_skill_4.pause_with_previous();
	return badguy;
}