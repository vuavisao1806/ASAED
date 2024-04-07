#include "object/player.hpp"

#include <SDL_image.h>

#include "badguy/badguy.hpp"
#include "control/controller.hpp"
#include "control/input_manager.hpp"
#include "video/layer.hpp"
#include "video/video_system.hpp"
#include "video/drawing_context.hpp"
#include "sprite/sprite.hpp"
#include "sprite/sprite_manager.hpp"
#include "math/util.hpp"

namespace {
	const float WALK_SPEED = 100.0f; // That funny setup because I don't think another more beautiful
} // namespace

Player::Player(int player_id, int weapon_id) :
	m_id(player_id),
	m_controller(&(InputManager::current()->get_controller(player_id))),
	m_direction(Direction::RIGHT),
	m_sprite(SpriteManager::current()->create("data/images/creatures/knight/knight-sprite.json"))
{
	set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
	set_pos(Vector(100.0f, 100.0f));
	
	m_weapon = WeaponSet::current()->get(weapon_id).clone(this);
	m_weapon->set_offset(m_weapon->get_bounding_box().get_middle());
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
	if (dynamic_cast<BadGuy*>(&other)) {
		collision_solid(hit);
		return FORCE_MOVE;
	}
	return CONTINUE;
}

void Player::collision_tile(uint32_t tile_attributes) {

}

bool Player::is_valid() const { return MovingObject::is_valid(); }

void Player::update(float dt_sec) {
	handle_input();

	set_movement(m_physic.get_movement(dt_sec));
}

void Player::draw(DrawingContext& drawing_context) {
	std::string suffix_action = (m_direction == Direction::RIGHT ? "-right" : "-left");
	if (math::length(m_physic.get_velocity()) < 1.0f) {
		m_sprite->set_action("idle" + suffix_action);
	}
	else {
		m_sprite->set_action("walk" + suffix_action);
	}
	m_sprite->draw(drawing_context.get_canvas(), get_pos(), get_layer());

	if (m_weapon) {
		m_weapon->draw(drawing_context);
	}
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
	Vector to_logical = VideoSystem::current()->get_viewport().to_logical(mouse_position.x, mouse_position.y) - get_bounding_box().get_middle();
	
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