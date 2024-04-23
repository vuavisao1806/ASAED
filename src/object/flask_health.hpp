#ifndef HEADER_ASAED_OBJECT_FLASK_HEALTH_HPP
#define HEADER_ASAED_OBJECT_FLASK_HEALTH_HPP

#include "object/moving_sprite.hpp"

#include "math/vector.hpp"
#include "util/timer.hpp"

/**
 * FlaskHealth to recovery health
 * FlaskHealth should inheritance GameObject but MovingSprite is more convenient
*/

class FlaskHealth final : public MovingSprite {
public:
	FlaskHealth(const Vector& position);

private:
	FlaskHealth(const FlaskHealth&) = delete;
	FlaskHealth& operator=(const FlaskHealth&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;

	virtual bool is_singleton() const override;

	static std::string class_name();
	virtual std::string get_class_name() const override;
};

#endif