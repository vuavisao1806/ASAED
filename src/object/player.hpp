#ifndef HEADER_ASAED_OBJECT_PLAYER_HPP
#define HEADER_ASAED_OBJECT_PLAYER_HPP


#include <SDL.h>
#include <memory>
#include <string>

#include "math/vector.hpp"
#include "object/physic.hpp"
#include "object/direction.hpp"
#include "object/moving_object.hpp"
#include "sprite/sprite_ptr.hpp"
#include "util/timer.hpp"
#include "weapon/weapon.hpp"
#include "weapon/weapon_set.hpp"

class Controller;
class DrawingContext;

class Player : public MovingObject {
private:
	int m_id;
	const Controller* m_controller;
	
	int m_health;
	Timer m_timer_dead;

	Direction m_direction;
	Physic m_physic;
	SpritePtr m_sprite;

	std::unique_ptr<Weapon> m_weapon;

public:
	Player(const Player& other);
	~Player() override;

public:
	Player(int player_id, int weapon_id);

private:
	Player& operator=(const Player&) = delete;

public:
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;

	virtual bool is_valid() const override;

	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
	
	virtual int get_layer() const override;

public:
	void handle_input(); // temporary, will be soon update
	/** specific handle movement */
	void handle_movement_input();
	/** specific handle attack */
	void handle_attack_input();

public:
	int get_id() const;
	void set_id(int id);

	std::unique_ptr<Player> clone() const;
};


#endif