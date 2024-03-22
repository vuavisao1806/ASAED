#include "object/player.hpp"

#include <SDL_image.h>

#include "video/video_system.hpp"
#include "video/layer.hpp"
#include "sprite/sprite_manager.hpp"

Player::Player() :
	pos(0, 0),
	m_movement(0, 0),
	m_sprite()
{}

Player::~Player() 
{}

Player::Player(int x, int y, std::string path) :
	pos(x, y)
{
	m_sprite = SpriteManager::current()->create(path);
}

void Player::moved(const Vector& add) {
	m_movement += add;
}

void Player::update() {
	pos += m_movement;
	m_movement = Vector();
}

void Player::draw(Canvas& canvas, bool go_left) {
	if (go_left) {
		m_sprite->set_action("walk-left");
	}
	else {
		m_sprite->set_action("walk-right");
	}
	m_sprite->draw(canvas, pos, LAYER_OBJECT);
}


