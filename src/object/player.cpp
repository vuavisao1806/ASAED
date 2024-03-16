#include "object/player.hpp"

#include <SDL_image.h>

#include "video/video_system.hpp"
#include "video/texture_manager.hpp"

Player::Player() :
	pos(0, 0),
	m_movement(0, 0),
	m_texture()
{}

Player::~Player() 
{}

Player::Player(int x, int y) :
	pos(x, y)
{}

void Player::loadTexture(std::string path) {
	m_texture = TextureManager::current()->get(path);
}

void Player::moved(const Vector& add) {
	m_movement += add;
}

void Player::update() {
	pos += m_movement;
	m_movement = Vector();
}


