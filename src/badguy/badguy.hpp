#ifndef HEADER_ASAED_BADGUY_BADGUY_HPP
#define HEADER_ASAED_BADGUY_BADGUY_HPP

#include "object/moving_sprite.hpp"

#include <memory>
#include <string>
#include <vector>

#include "object/direction.hpp"
#include "object/physic.hpp"
#include "math/vector.hpp"
#include "util/timer.hpp"

enum BadGuyData {
	OGRE,
	WIZZARD
};

/** Class BadGuy (base) to help inheritance and manage all of types badguy */

class BadGuy : public MovingSprite {
protected:
	enum State {
		/** state didn't see the player */
		STATE_INACTIVE,
		/** state didn't the player */
		STATE_ACTIVE
	};

protected:
	Vector m_start_position;

	int m_health;
	Timer m_timer_dead;

	int m_hurt;
	
	float m_radius_wander;
	Timer m_timer_wander;

	Timer m_timer_shoot;
	State m_state;

	Direction m_direction;
	Physic m_physic;

	std::vector<Vector> m_smart_position;

public:
	BadGuy(const std::string& filename);

private:
	BadGuy(const BadGuy&) = delete;
	BadGuy& operator=(const BadGuy&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;

	virtual void activated();
	virtual void deactivated();
	virtual void active_update(float dt_sec);
	virtual void inactive_update(float dt_sec);
	
	virtual void wandering();
	virtual bool is_boss() const = 0;

	virtual std::unique_ptr<BadGuy> clone(const Vector& pos) const = 0;

public:
	void set_state(State state);
	State get_state() const;

	void set_start_position(Vector start_position);
	Vector get_start_position() const;

protected:
	void try_active();
};

#endif