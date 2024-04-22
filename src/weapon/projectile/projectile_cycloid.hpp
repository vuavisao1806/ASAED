#ifndef HEADER_ASAED_PROJECTILE_PROJECTILE_CYCLOID_HPP
#define HEADER_ASAED_PROJECTILE_PROJECTILE_CYCLOID_HPP

#include "weapon/projectile/projectile.hpp"

class ProjectileCycloid : public Projectile {
private:
	Vector m_first_position;
	float m_timer; // bad design, but if it's work, it's oke

public:
	ProjectileCycloid(const std::string& filename);

private:
	ProjectileCycloid(const ProjectileCycloid&) = delete;
	ProjectileCycloid& operator=(const ProjectileCycloid&) = delete;

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