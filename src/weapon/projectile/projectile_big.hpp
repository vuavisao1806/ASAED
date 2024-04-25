#ifndef HEADER_ASAED_WEAPON_WEAPON_PROJECTILE_BIG_HPP
#define HEADER_ASAED_WEAPON_WEAPON_PROJECTILE_BIG_HPP

#include "weapon/projectile/projectile.hpp"

#include <string>

#include "util/timer.hpp"

class ReaderData;


class ProjectileBig : public Projectile {
private:
	Timer m_timer_spawn;

public:
	ProjectileBig(const std::string& filename);

private:
	ProjectileBig(const ProjectileBig&) = delete;
	ProjectileBig& operator=(const ProjectileBig&) = delete;

public:
	static std::unique_ptr<Projectile> from_file(const ReaderData* data);

public:
	virtual void collision_tile(uint32_t tile_attributes) override;

	virtual void update(float dt_sec) override;
	
	static std::string class_name();
	virtual std::string get_class_name() const override;

	virtual std::unique_ptr<Projectile> clone(const Vector& pos, uint32_t hurt_attributes, float angle, float angle_shift = std::numeric_limits<float>::max()) const override;
};

#endif