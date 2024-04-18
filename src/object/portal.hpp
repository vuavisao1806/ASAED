#ifndef HEADER_ASAED_OBJECT_PORTAL_HPP
#define HEADER_ASAED_OBJECT_PORTAL_HPP

#include "object/moving_sprite.hpp"

#include "math/vector.hpp"
#include "util/timer.hpp"

/**
 * Portal to transition level
 * Portal should inheritance GameObject but MovingSprite is more convenient
*/

class Portal final : public MovingSprite {
private:
	Timer m_timer;
	
public:
	Portal(const Vector& position);

private:
	Portal(const Portal&) = delete;
	Portal& operator=(const Portal&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;

	virtual bool is_singleton() const override;

	static std::string class_name();
	virtual std::string get_class_name() const override;
};

#endif