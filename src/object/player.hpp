#ifndef HEADER_ASAED_OBJECT_PLAYER_HPP
#define HEADER_ASAED_OBJECT_PLAYER_HPP

#include "math/vector.hpp"
#include "math/rect.hpp"
#include "math/rectf.hpp"
#include "math/size.hpp"

#include <SDL.h>
#include <string>

#include "video/texture_ptr.hpp"
#include "video/texture.hpp"

class Player {
public:
	Vector pos;

public:
	// m_movement also temporary. But it will serve for something in the future 
	Vector m_movement;
	
	// Now I only load 1 texture. It's temporary, I will upgrade!
	TexturePtr m_texture;

public:
	Player();
	~Player();

	Player(int x, int y);

public:
	void loadTexture(std::string path);

public:
	void moved(const Vector& add);
	void update();
};


#endif