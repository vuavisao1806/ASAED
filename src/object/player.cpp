#include "object/player.hpp"

#include <SDL_image.h>
#include <algorithm>

#include "asaed/room.hpp"
#include "asaed/resources.hpp"
#include "badguy/badguy.hpp"
#include "control/controller.hpp"
#include "control/input_manager.hpp"
#include "object/camera.hpp"
#include "object/chest.hpp"
#include "object/floating_text.hpp"
#include "object/flask_health.hpp"
#include "gui/colorscheme.hpp"
#include "math/util.hpp"
#include "math/rectf.hpp"
#include "math/random.hpp"
#include "weapon/hurt.hpp"
#include "weapon/projectile/projectile.hpp"
#include "video/video_system.hpp"
#include "video/drawing_context.hpp"
#include "video/surface.hpp"
#include "sprite/sprite.hpp"
#include "sprite/sprite_manager.hpp"
namespace {
	const float WALK_SPEED = 100.0f; // That funny setup because I don't think another more beautiful
	
	const float SHIELD_RECOVERY = 3.0f;
	const float TIME_DEAD = 2.5f;

	const int HEALTH = 5;
	const int SHIELD = 5;

	const float HEALTH_BAR_WIDTH = 100.0f;
	const float HEALTH_BAR_HEIGHT = 30.0f;
} // namespace

Player::Player(int player_id, int weapon_id) :
	m_id(player_id),
	m_controller(&(InputManager::current()->get_controller(player_id))),
	m_health(HEALTH),
	m_shield(SHIELD),
	m_timer_dead(),
	m_direction(Direction::RIGHT),
	m_sprite(SpriteManager::current()->create("data/images/creatures/knight/knight-sprite.json")),
	m_weapon(WeaponSet::current()->get(GATLING_PLAYER).clone(this)) // tested
{
	set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
	set_pos(Vector(100.0f, 100.0f));
	m_weapon->set_offset(m_weapon->get_bounding_box().get_middle());

	m_timer_shield.start(SHIELD_RECOVERY, true);

	m_timer_dead.start(TIME_DEAD, true);
	m_timer_dead.pause_with_previous();
}


Player::Player(const Player& other) :
	m_id(other.m_id),
	m_controller(other.m_controller),
	m_health(other.m_health),
	m_shield(other.m_shield),
	m_timer_dead(),
	m_direction(other.m_direction),
	m_sprite(other.m_sprite->clone()),
	m_weapon(other.m_weapon->clone(this))
{
	set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
	set_pos(other.get_pos());

	m_timer_shield.start(SHIELD_RECOVERY, true);

	m_timer_dead.start(TIME_DEAD, true);
	m_timer_dead.pause_with_previous();
}

Player::~Player() 
{}

void Player::collision_solid(const CollisionHit& hit) {
	if (hit.left || hit.right) {
		m_physic.set_velocity_x(0);
	}
	if (hit.top || hit.bottom) {
		m_physic.set_velocity_y(0);
	}
}

HitResponse Player::collision(CollisionObject& other, const CollisionHit& hit) {
	if (auto bullet = dynamic_cast<Projectile*>(&other)) {
		if ((bullet->get_hurt_attributes() & HURT_PLAYER)) {
			int damage[2] = {0, 0};
			damage[0] = std::min(bullet->get_damage(), m_shield);
			damage[1] = bullet->get_damage() - damage[0];
			m_shield -= damage[0];
			m_health -= damage[1];

			Vector position = Vector(g_game_random.randf(get_bounding_box().get_left(), get_bounding_box().get_right()),
			                         g_game_random.randf(get_bounding_box().get_top(), get_bounding_box().get_bottom()));
			Room::get().add<FloatingText>(position, bullet->get_damage());
		}
		return ABORT_MOVE;
	}

	if (dynamic_cast<BadGuy*>(&other)) {
		return CONTINUE;
	}

	if (dynamic_cast<FlaskHealth*>(&other)) {
		if (m_controller->pressed(Control::ATTACK)) {
			m_health = std::min(HEALTH, m_health + 1); // noooo!
		}
	}

	if (auto* chest = dynamic_cast<Chest*>(&other)) {
		if (m_controller->pressed(Control::ATTACK)) {
			switch_weapon(*chest);
		}
	}
	return CONTINUE;
}

void Player::collision_tile(uint32_t tile_attributes) {

}

bool Player::is_valid() const { return MovingObject::is_valid(); }

void Player::update(float dt_sec) {
	if (m_health <= 0) {
		if (m_timer_dead.paused()) {
			set_group(COLLISION_DISABLED);
			m_timer_dead.start_with_previous();

			std::string suffix_action = (m_direction == Direction::RIGHT ? "-right" : "-left");
			m_sprite->set_action("dead" + suffix_action);
		}
		if (m_timer_dead.check()) {
			remove_me();
		}
		return;
	}

	if (m_timer_shield.check()) {
		m_shield = std::min(m_shield + 1, SHIELD);
	}

	handle_input();

	set_movement(m_physic.get_movement(dt_sec));
}

void Player::draw(DrawingContext& drawing_context) {
	if (m_health <= 0) {
		m_health = 0;
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


	{
		// 100% hardcode

		// draw HUD
		Vector position = Room::get().get_camera().get_translation();
		Sizef size = Sizef(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);
		// frame
		drawing_context.get_canvas().draw_filled_rect(Rectf(position, size), ColorScheme::HUD::frame_back, LAYER_HUD);
		position += Vector(1.0f, 1.0f);
		drawing_context.get_canvas().draw_filled_rect(Rectf(position, size - Sizef(2.0f, 2.0f)), ColorScheme::HUD::frame_front, LAYER_HUD);

		position += Vector(1.0f, 1.0f);

		auto draw_item = [&] (Vector& position, const SurfacePtr& surface, const TTFFontPtr& font,
		                      const Color& front, const Color& back,
		                      int m_value, int CAPACITY) -> void {
			Vector item_position = position;
			const float item_width = surface->get_width();
			const float item_height = surface->get_height();
			Sizef item_size = Sizef(size.width - 2.0f, item_height);
			drawing_context.get_canvas().draw_surface(surface, item_position, LAYER_HUD);

			item_position.x += item_width + 4.0f;
			item_size -= Sizef(item_width + 5.0f, 0.0f);
			float rem = item_size.width / CAPACITY;

			drawing_context.get_canvas().draw_filled_rect(Rectf(item_position, item_size), ColorScheme::HUD::space_back, LAYER_GUI);
			
			item_size.height /= 2.0f;
			drawing_context.get_canvas().draw_filled_rect(Rectf(item_position, item_size - Sizef(rem, 0.0f) * (CAPACITY - m_value)), front, LAYER_GUI);

			item_position.y += item_size.height;
			drawing_context.get_canvas().draw_filled_rect(Rectf(item_position, item_size - Sizef(rem, 0.0f) * (CAPACITY - m_value)), back, LAYER_GUI);
			
			std::string item = std::to_string(m_value) + '/' + std::to_string(CAPACITY);
			item_position.y -= item_size.height + 1.0f;
			item_position.x += item_size.width / 2.0f - font->get_text_width(item) / 2.0f;
			drawing_context.get_canvas().draw_text(font, item, item_position, ALIGN_LEFT, LAYER_GUI, ColorScheme::Text::small_color);

			position.y += item_height + 1.0f;
		};

		draw_item(position, Resources::heart, Resources::small_font, ColorScheme::HUD::heart_front, ColorScheme::HUD::heart_back, m_health, HEALTH);
		draw_item(position, Resources::shield, Resources::small_font, ColorScheme::HUD::shield_front, ColorScheme::HUD::shield_back, m_shield, SHIELD);
	}

	m_sprite->draw(drawing_context.get_canvas(), get_pos(), get_layer());

}

void Player::handle_input() {
	handle_movement_input();
	handle_attack_input();
}

void Player::handle_movement_input() {
	float m_dir_x = 0.0f;
	if (m_controller->hold(Control::LEFT) && !m_controller->hold(Control::RIGHT)) {
		m_dir_x = -1.0f;
		m_direction = Direction::LEFT;
	}
	else if (!m_controller->hold(Control::LEFT) && m_controller->hold(Control::RIGHT)) {
		m_dir_x = 1.0f;
		m_direction = Direction::RIGHT;
	}

	float m_dir_y = 0.0f;
	if (m_controller->hold(Control::UP) && !m_controller->hold(Control::DOWN)) {
		m_dir_y = -1.0f;
	}
	else if (!m_controller->hold(Control::UP) && m_controller->hold(Control::DOWN)) {
		m_dir_y = 1.0f;
	}
	
	m_physic.set_velocity(Vector(m_dir_x, m_dir_y) * WALK_SPEED);
}

void Player::handle_attack_input() {
	Vector mouse_position = m_controller->get_cursor_position();
	Vector to_logical = VideoSystem::current()->get_viewport().to_logical(mouse_position.x, mouse_position.y)
	                    - (get_bounding_box().get_middle() - Room::get().get_camera().get_translation());
	float angle = math::angle(math::normalize(to_logical));

	m_weapon->set_angle(angle);
	
	if(std::fabs(angle) >= 90.0f) {
		m_weapon->set_flip(VERTICAL_FLIP);
	}
	else {
		m_weapon->set_flip(NO_FLIP);
	}
	
	if (m_weapon) {
		if (m_controller->hold(Control::ATTACK)) {
			m_weapon->attack();
		}
	}
}

int Player::get_id() const { return m_id; };
void Player::set_id(int id) {
	m_id = id;
	m_controller = &(InputManager::current()->get_controller(m_id));
}

int Player::get_layer() const { return LAYER_OBJECT + 1; }

void Player::switch_weapon(Chest& chest) {
	if (!chest.get_weapon()) {
		return;
	}
	auto new_weapon = chest.get_weapon()->clone(this);

	chest.set_weapon(m_weapon->clone(&chest));
	
	m_weapon = std::move(new_weapon);
	m_weapon->set_offset(m_weapon->get_bounding_box().get_middle());
}

std::unique_ptr<Player> Player::clone() const {
	assert(this != nullptr);
	auto player = std::make_unique<Player>(*this);
	return player;
}