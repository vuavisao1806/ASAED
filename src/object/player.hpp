#ifndef HEADER_ASAED_OBJECT_PLAYER_HPP
#define HEADER_ASAED_OBJECT_PLAYER_HPP

#include "math/vector.hpp"
#include "math/rect.hpp"
#include "math/rectf.hpp"
#include "math/size.hpp"

#include <SDL.h>
#include <string>

#include "sprite/sprite_ptr.hpp"
#include "sprite/sprite.hpp"

class Player {
public:
	Vector pos;

public:
	// m_movement also temporary. But it will serve for something in the future 
	Vector m_movement;
	
	// Finally! I can load sprite
	SpritePtr m_sprite;
public:
	Player();
	~Player();

	Player(int x, int y, std::string path = "");

public:
	void moved(const Vector& add);
	void update();

	void draw(Canvas& canvas, int go, int last_go);
};


#endif