#ifndef HEADER_ASAED_OBJECT_PLAYER_HPP
#define HEADER_ASAED_OBJECT_PLAYER_HPP

#include "math/vector.hpp"
#include "math/rect.hpp"
#include "math/rectf.hpp"
#include "math/size.hpp"

#include <SDL.h>
#include <string>

class Player {
public:
	Vector pos;
	Size size;

public:
	// m_movement also temporary. But it will serve for something in the future 
	Vector m_movement;
	
	// Now I only load 1 texture. It's temporary, I will upgrade!
	SDL_Texture* m_texture;

public:
	Player();
	~Player();

	Player(int x, int y, Size sz);

public:
	void loadTexture(SDL_Renderer* renderer, std::string path);

public:
	void moved(const Vector& add);
	void update();
};


#endif