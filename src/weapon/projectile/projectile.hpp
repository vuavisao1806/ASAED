#ifndef HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_HPP
#define HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_HPP

#include "object/moving_sprite.hpp"
#include "object/physic.hpp"

#include <stdint.h>
#include <string>

/**
 * Class Projectile holds specific objects that can cause damage, ... when reaching other objects
 * It can be used for bullet in guns (more flexible)
*/

enum ProjectileData {
	AK47_PLAYER,
	AK47_BADGUY,
	SGUN_PLAYER,
	SGUN_BADGUY,
	GATLING_PLAYER,
	GATLING_BADGUY,
	BOOMERANG_BADGUY
};

class Projectile : public MovingSprite {
protected:
	uint32_t m_hurt_attributes;
	Physic m_physic;
	int m_damage;
	float m_ratio_crit;

public:
	Projectile(const std::string& filename);

public:
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;
	
	virtual void update(float dt_sec) override;

	static std::string class_name();
	virtual std::string get_class_name() const override;
	
	virtual std::unique_ptr<Projectile> clone(const Vector& pos, uint32_t hurt_attributes, float angle) const = 0;

public:
	uint32_t get_hurt_attributes() const;
	void set_hurt_attributes(uint32_t hurt_attributes);

	int get_damage() const;
	float get_ratio_crit() const;
};

#endif