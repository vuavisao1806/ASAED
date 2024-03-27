#ifndef HEADER_ASAED_OBJECT_PLAYER_HPP
#define HEADER_ASAED_OBJECT_PLAYER_HPP


#include <SDL.h>
#include <string>

#include "math/vector.hpp"
#include "object/physic.hpp"
#include "object/direction.hpp"
#include "sprite/sprite_ptr.hpp"

class Controller;
class DrawingContext;

class Player {
private:
	Vector pos; // temporary, will be soon update
	
	int m_id;
	const Controller* m_controller;
	
	Direction m_direction;
	Physic m_physic;
	SpritePtr m_sprite;

public:
	~Player();

public:
	Player(int player_id); // temporary, will be soon update

private:
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

public:
	void update(float dt_sec); // temporary, will be soon override
	void draw(DrawingContext& drawing_context); // temporary, will be soon override

public:
	void handle_input(); // temporary, will be soon update
	/** specific handle movement */
	void handle_movement_input();

public:
	int get_id() const;
	void set_id(int id);
};


#endif