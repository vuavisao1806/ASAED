#ifndef HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_BOOMERANG_HPP
#define HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_BOOMERANG_HPP

#include "weapon/projectile/projectile.hpp"

#include <string>

#include "util/timer.hpp"

class ReaderData;


class ProjectileBoomerang : public Projectile {
private:
	int m_turn;
	int m_turn_damage;
	Timer m_timer_go_back;

public:
	ProjectileBoomerang(const std::string& filename);

private:
	ProjectileBoomerang(const ProjectileBoomerang&) = delete;
	ProjectileBoomerang& operator=(const ProjectileBoomerang&) = delete;

public:
	static std::unique_ptr<Projectile> from_file(const ReaderData* data);

public:
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual HitResponse collision(CollisionObject& other, const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;

	virtual void update(float dt_sec) override;
	
	static std::string class_name();
	virtual std::string get_class_name() const override;

	virtual std::unique_ptr<Projectile> clone(const Vector& pos, uint32_t hurt_attributes, float angle, float angle_shift = std::numeric_limits<float>::max()) const override;
};

#endif