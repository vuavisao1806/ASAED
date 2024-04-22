#ifndef HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_CYCLE_HPP
#define HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_CYCLE_HPP

#include "weapon/projectile/projectile.hpp"

#include <string>

#include "math/vector.hpp"

class ReaderData;


class ProjectileCycle : public Projectile {
private:
	Vector m_shift;

public:
	ProjectileCycle(const std::string& filename);

private:
	ProjectileCycle(const ProjectileCycle&) = delete;
	ProjectileCycle& operator=(const ProjectileCycle&) = delete;

public:
	static std::unique_ptr<Projectile> from_file(const ReaderData* data);

public:
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;

	virtual void update(float dt_sec) override;
	
	static std::string class_name();
	virtual std::string get_class_name() const override;

	virtual std::unique_ptr<Projectile> clone(const Vector& pos, uint32_t hurt_attributes, float angle) const override;
};

#endif