#ifndef HEADER_ASAED_OBJECT_CHEST_HPP
#define HEADER_ASAED_OBJECT_CHEST_HPP

#include "object/moving_sprite.hpp"

#include "math/vector.hpp"

/**
 * Chest to hold chest :>
 * Chest should inheritance GameObject but MovingSprite is more convenient
*/

class Chest final : public MovingSprite {
public:
	Chest(const Vector& position);

private:
	Chest(const Chest&) = delete;
	Chest& operator=(const Chest&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	virtual bool is_singleton() const override;
};

#endif