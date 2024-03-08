#include "object/player.hpp"

#include <SDL_image.h>

Player::Player() :
	pos(0, 0),
	size(0, 0),
	m_movement(0, 0)
{
	m_texture = nullptr;
}

Player::~Player() {
	if (m_texture != nullptr) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}
	pos = Vector(0, 0);
	m_movement = Vector(0, 0);
}

Player::Player(int x, int y, Size sz) :
	pos(x, y),
	size(sz),
	m_movement(0, 0)
{
	m_texture = nullptr;
}

void Player::loadTexture(SDL_Renderer* renderer, std::string path) {
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == nullptr) {
		throw "can not load surface";
	}
	// size = Size(tempSurface->w, tempSurface->h);
	m_texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (m_texture == nullptr) {
		throw "can not load texture";
	}
	SDL_FreeSurface(tempSurface);
}

void Player::moved(const Vector& add) {
	m_movement += add;
}

void Player::update() {
	pos += m_movement;
	m_movement = Vector();
}


