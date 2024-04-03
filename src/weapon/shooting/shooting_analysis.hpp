#ifndef HEADER_ASAED_SHOOTING_SHOOTING_ANALYSIS_HPP
#define HEADER_ASAED_SHOOTING_SHOOTING_ANALYSIS_HPP

#include "weapon/shooting/shooting.hpp"

#include <string>
#include <stdint.h>

#include "math/vector.hpp"

class ReaderData;

class ShootingAnalysis : public Shooting {
private:
	uint32_t m_moving_tile_id;
	Vector m_spawn_position;
	
public:
	ShootingAnalysis(const std::string& filename);

private:
	ShootingAnalysis(const ShootingAnalysis&) = delete;
	ShootingAnalysis& operator=(const ShootingAnalysis&) = delete;

public:
	static std::unique_ptr<Weapon> from_file(const ReaderData* data);

public:
	virtual uint32_t get_moving_tile_id() const override;
	virtual Vector get_spawn_position() const override;
	virtual float get_shoot_angle() const override;

	static std::string class_name();
	virtual std::string get_class_name() const override;

	virtual std::unique_ptr<Weapon> clone(MovingObject* parent, const Vector& pos) const override;
};

#endif