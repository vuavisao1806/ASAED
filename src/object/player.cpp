#include "object/player.hpp"

#include <SDL_image.h>

#include "video/video_system.hpp"
#include "video/texture_manager.hpp"

Player::Player() :
	pos(0, 0),
	m_movement(0, 0),
	m_surface()
{}

Player::~Player() 
{}

Player::Player(int x, int y, std::string path) :
	pos(x, y)
{
	m_surface = Surface::from_file(path);
}

void Player::moved(const Vector& add) {
	m_movement += add;
}

void Player::update() {
	pos += m_movement;
	m_movement = Vector();
}


