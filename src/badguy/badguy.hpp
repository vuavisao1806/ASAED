#ifndef HEADER_ASAED_BADGUY_BADGUY_HPP
#define HEADER_ASAED_BADGUY_BADGUY_HPP

#include "object/moving_sprite.hpp"

#include <memory>
#include <string>

#include "object/direction.hpp"
#include "object/physic.hpp"
#include "math/vector.hpp"
#include "util/timer.hpp"

enum BadGuyData {
	OGRE,
	MAGICIAN
};

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
	
	float m_radius_wander;
	Timer m_timer_wander;

	float m_radius_detect;

	Timer m_timer_shoot;
	State m_state;

	Direction m_direction;
	Physic m_physic;

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

	virtual std::unique_ptr<BadGuy> clone(const Vector& pos) const = 0;

public:
	void set_state(State state);
	State get_state() const;

protected:
	void try_active();
};

#endif