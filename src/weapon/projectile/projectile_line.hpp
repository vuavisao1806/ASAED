#ifndef HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_LINE_HPP
#define HEADER_ASAED_WEAPON_PROJECTILE_PROJECTILE_LINE_HPP

#include "weapon/projectile/projectile.hpp"

#include <string>

class ReaderData;


class ProjectileLine : public Projectile {
private:
	int m_wall_bounce_count;

	int m_color; // for fun, only use in Gatling

public:
	ProjectileLine(const std::string& filename);

private:
	ProjectileLine(const ProjectileLine&) = delete;
	ProjectileLine& operator=(const ProjectileLine&) = delete;

public:
	static std::unique_ptr<Projectile> from_file(const ReaderData* data);

public:
	virtual void collision_solid(const CollisionHit& hit) override;
	virtual void collision_tile(uint32_t tile_attributes) override;
	
	static std::string class_name();
	virtual std::string get_class_name() const override;

	virtual std::unique_ptr<Projectile> clone(const Vector& pos, uint32_t hurt_attributes, float angle, float angle_shift = std::numeric_limits<float>::max()) const override;
};

#endif