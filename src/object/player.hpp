#ifndef HEADER_ASAED_OBJECT_PLAYER_HPP
#define HEADER_ASAED_OBJECT_PLAYER_HPP

#include "math/vector.hpp"
#include "math/rect.hpp"
#include "math/rectf.hpp"
#include "math/size.hpp"

#include <SDL.h>
#include <string>

#include "video/surface_ptr.hpp"
#include "video/surface.hpp"

class Player {
public:
	Vector pos;

public:
	// m_movement also temporary. But it will serve for something in the future 
	Vector m_movement;
	
	// Now I only load 1 texture. It's temporary, I will upgrade!
	SurfacePtr m_surface;

public:
	Player();
	~Player();

	Player(int x, int y, std::string path = "");

public:
	void moved(const Vector& add);
	void update();
};


#endif