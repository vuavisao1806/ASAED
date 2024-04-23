#ifndef HEADER_ASAED_OBJECT_CHEST_HPP
#define HEADER_ASAED_OBJECT_CHEST_HPP

#include "object/moving_sprite.hpp"

#include <memory>

#include "object/flask_health.hpp"
#include "math/vector.hpp"
#include "weapon/weapon.hpp"
#include "util/timer.hpp"

/**
 * Chest to hold chest :>
 * Chest should inheritance GameObject but MovingSprite is more convenient
*/

enum ChessType {
	FlaskHealthType,
	WeaponType
};

class Chest final : public MovingSprite {
private:
	std::unique_ptr<Weapon> m_weapon;
	bool m_spawn;

public:
	Chest(const Vector& position);

private:
	Chest(const Chest&) = delete;
	Chest& operator=(const Chest&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;

	virtual bool is_singleton() const override;

public:
	void set_weapon(std::unique_ptr<Weapon> weapon);
	const std::unique_ptr<Weapon>& get_weapon();
};

#endif