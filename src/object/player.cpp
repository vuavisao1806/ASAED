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

void Player::draw(Canvas& canvas, int go, int last_go) {
	if (go == -1) {
		m_sprite->set_action("walk-left");
	}
	else if(go == 1) {
		m_sprite->set_action("walk-right");
	}
	else if (last_go == 1) {
		m_sprite->set_action("idle-right");
	}
	else {
		m_sprite->set_action("idle-left");
	}
	m_sprite->draw(canvas, pos, LAYER_OBJECT);
}


